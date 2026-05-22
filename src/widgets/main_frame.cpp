#include "widgets/main_frame.hpp"

MainFrame::MainFrame(QWidget* parent) : QFrame(parent) {
	_defaultArgs = { "-o", "json", "--no-pager" };
	auto layout = new QVBoxLayout();
	auto filterFrame = new FilterFrame();
	_filterStringList = QStringList();
	connect(filterFrame, &FilterFrame::update, this, &MainFrame::setFilterStringList);
	_table = new QTableView(this);
	_model = new JournalModel(this);
	_bootWidget = new BootWidget(this, _boot);
	connect(_bootWidget, &BootWidget::bootSet, this, &MainFrame::updateBoot);
	_table->setModel(_model);

	// Putting this here to be clear and avoid magic numbers
	int timeColIndex = 0;
	int timeColSize = 180;
	_table->setColumnWidth(timeColIndex, timeColSize);
	int priorityColIndex = 1;
	int hostColIndex = 3;
	int smallColSize = 100;
	for (auto& index : { priorityColIndex, hostColIndex }) {
		_table->setColumnWidth(index, smallColSize);
	}
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
		auto const stderrMessage = _process->readAllStandardError().toStdString();
		auto const errorMessage = "There was an error while fetching data from journalctl:\n" + stderrMessage + "\n\nExit code: " + std::to_string(exitCode);
		qDebug() << errorMessage << "\n";
		auto messageBox = QMessageBox(QMessageBox::Icon::Critical, "Error fetching journal data", QString::fromUtf8(errorMessage), QMessageBox::StandardButton::Ok, this);
		messageBox.exec();
		return;
	}

	_table->viewport()->update();
}

void MainFrame::reload() {
	if (_process->state() != QProcess::NotRunning)
		_process->kill();
	_model->clear();
	QStringList args;
	if (_filterStringList.size() != 0 && !_filterStringList.first().isEmpty())
		args += _filterStringList;
	for (auto& item : _defaultArgs) {
		args.append(item);
	}

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
}

void MainFrame::showDetails(const QModelIndex& index) {
	const auto data = _model->getFullData(index);
	const auto details = _model->getExtraDetails(index);
	const auto icon = _model->revealIcon(index);
	DetailsView* detailsView = new DetailsView(icon, data, details);
	detailsView->show();
}

MainFrame::~MainFrame() = default;