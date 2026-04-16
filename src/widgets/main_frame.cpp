#include "widgets/main_frame.hpp"

MainFrame::MainFrame(QWidget* parent) : QFrame(parent) {
	_defaultArgs = { "-o", "json", "--no-pager" };
	auto layout = new QVBoxLayout();
	auto filterFrame = new FilterFrame();
	connect(filterFrame, &FilterFrame::update, this, &MainFrame::setFilterStringList);
	_table = new QTableView(this);
	_model = new JournalModel(this);
	_bootWidget = new BootWidget(this, _boot);
	connect(_bootWidget, &BootWidget::bootSet, this, &MainFrame::updateBoot);
	_table->setModel(_model);
	_table->horizontalHeader()->setStretchLastSection(true);
	_table->setSelectionBehavior(QAbstractItemView::SelectRows);
	connect(_table, &QTableView::doubleClicked, this, &MainFrame::showDetails);

	_progressBar = new QProgressBar(this);
	_progressBar->setRange(0, 0);
	_progressFrame = new QFrame(this);
	auto progressFrameLayout = new QHBoxLayout(_progressFrame);
	progressFrameLayout->addWidget(_progressBar);
	_progressFrame->setLayout(progressFrameLayout);

	layout->addWidget(filterFrame);
	layout->addWidget(_table, 20);
	layout->addWidget(_progressFrame, 20);
	layout->addWidget(_bootWidget);
	setLayout(layout);
	_table->setHidden(false);
	_progressFrame->setHidden(true);
	_process = new QProcess(this);
	connect(_process, &QProcess::finished, this, &MainFrame::onProcessFinished);
	connect(_process, &QProcess::readyReadStandardOutput, this, &MainFrame::readOutput);
	/*QStringList args = _defaultArgs;
	args.append("-n");
	args.append(std::to_string(_numberOfItems).c_str());
	_process->start("journalctl", args);*/
	reload();
}

void MainFrame::updateCursor(int page) {
	if (page < _page) {
		_moveToPreviousPage = true;
	} else if (page > _page) {
		_moveToNextPage = true;
	}
	_page = page;
	reload();
}

void MainFrame::updateNumberOfItems(int numberOfItems) {
	_numberOfItems = numberOfItems;
	reload();
}

void MainFrame::setFilterStringList(const QStringList& list) {
	_filterStringList = list;
	reload();
}

void MainFrame::updateBoot(int boot) {
	_boot = boot;
	reload();
}

void MainFrame::toggleProgressVisibility() {
	_table->setHidden(!_table->isHidden());
	_progressFrame->setHidden(!_progressFrame->isHidden());
}

void MainFrame::onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus) {
	toggleProgressVisibility();
	if (exitStatus == QProcess::CrashExit) {
		qDebug() << "Process crashed";
		return;
	}

	// Update your view
	_table->viewport()->update();
}

void MainFrame::reload() {
	if (_process->state() != QProcess::NotRunning)
		_process->kill();
	_model->clear();
	QStringList args;
	args += _filterStringList;
	for (auto& item : _defaultArgs) {
		args.append(item);
	}

	/*if (_moveToNextPage) {
		args.append("--after-cursor");
		args.append(_newestCursor);
	} else if (_moveToPreviousPage && _page > 1) {
		args.append("--cursor");
		args.append(_oldestCursor);
	}

	args.append("-n");
	args.append(std::to_string(_numberOfItems).c_str());*/
	auto bootStr = std::to_string(_boot);
	if (_boot != 0) {
		bootStr = "-" + bootStr;
	}
	args.append(("-b" + bootStr).c_str());
	for (auto& arg : args) {
		qDebug() << arg << " ";
	}
	toggleProgressVisibility();

	_process->start("journalctl", args);
}

void MainFrame::readOutput() {
	while (_process->canReadLine()) {
		QByteArray line = _process->readLine();
		QJsonParseError err;
		QJsonDocument doc = QJsonDocument::fromJson(line, &err);
		if (err.error != QJsonParseError::NoError) continue;
		QJsonObject o = doc.object();

		// Get priority string
		int priorityInt = std::atoi(o.value("PRIORITY").toString().toUtf8());
		const QMetaObject* metaObj = &PriorityEnum::staticMetaObject;
		int enumIndex = metaObj->indexOfEnumerator("Priority");
		QMetaEnum metaEnum = metaObj->enumerator(enumIndex);
		QString priorityStr = QString::fromUtf8(metaEnum.valueToKey(priorityInt));

		// Get time
		std::int64_t timeStampLong = std::stoll(o.value("__REALTIME_TIMESTAMP").toString().toStdString());
		time_t timeStamp = timeStampLong / 1'000'000;
		std::chrono::sys_time<std::chrono::milliseconds> chronoTimeStamp{ std::chrono::seconds{timeStamp} };
		std::chrono::local_time<std::chrono::milliseconds> localTimeStamp = std::chrono::current_zone()->to_local(chronoTimeStamp);
		std::string timeStampStr = std::format("{:%Y-%m-%d %H:%M:%S}", localTimeStamp);

		JournalEntry e;
		e.timestamp = timeStampStr.c_str();
		e.priority = priorityStr;
		e.unit = o.value("_SYSTEMD_UNIT").toString();
		e.hostname = o.value("_HOSTNAME").toString();
		e.message = o.value("MESSAGE").toString();
		e.cursor = o.value("__CURSOR").toString();

		// store other fields
		for (auto it = o.begin(); it != o.end(); ++it) {
			QString key = it.key();
			if (!e.extra.contains(key) &&
					key != "__REALTIME_TIMESTAMP" &&
					key != "PRIORITY" &&
					key != "_SYSTEMD_UNIT" &&
					key != "_HOSTNAME" &&
					key != "MESSAGE" &&
					key != "__CURSOR") {
				e.extra[key] = it.value().toString();
			}
		}
		_model->appendEntry(e);
	}
	_newestCursor = _model->getNewestCursor();
	_oldestCursor = _model->getOldestCursor();
}

void MainFrame::showDetails(const QModelIndex& index) {
	const auto data = _model->getFullData(index);
	const auto details = _model->getExtraDetails(index);
	const auto icon = _model->revealIcon(index);
	DetailsView* detailsView = new DetailsView(icon, data, details);
	detailsView->show();
}

MainFrame::~MainFrame() = default;