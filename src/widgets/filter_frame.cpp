#include "widgets/filter_frame.hpp"


FilterFrame::FilterFrame(QWidget* parent) {
	auto layout = new QHBoxLayout();
	auto calendarSinceButton = new QPushButton("Since", this);
	auto calendarUntilButton = new QPushButton("Until", this);

	connect(calendarSinceButton, &QPushButton::clicked, this, &FilterFrame::selectSinceDate);
	connect(calendarUntilButton, &QPushButton::clicked, this, &FilterFrame::selectUntilDate);

	_stringEdit = new QLineEdit(this);
	_stringEdit->setPlaceholderText("Regular expression search");
	_stringEdit->setMaximumHeight(28);
	_submitButton = new QPushButton("Submit", this);
	connect(_submitButton, &QPushButton::clicked, this, &FilterFrame::submitFilterString);
	connect(_stringEdit, &QLineEdit::returnPressed, this, &FilterFrame::submitFilterString);
	const QMetaObject* metaObj = &PriorityEnum::staticMetaObject;
	int enumIndex = metaObj->indexOfEnumerator("Priority");
	QMetaEnum metaEnum = metaObj->enumerator(enumIndex);

	_prioritySelect = new QComboBox;
	_prioritySelect->addItem("NO PRIORITY", -1);
	for (int i = 0; i < metaEnum.keyCount(); i++) {
		QString key = metaEnum.key(i);
		_prioritySelect->addItem(key, metaEnum.value(i));
	}
	connect(_prioritySelect, &QComboBox::currentIndexChanged, this, &FilterFrame::selectPriority);
	layout->addWidget(calendarSinceButton);
	layout->addWidget(calendarUntilButton);
	layout->addWidget(_stringEdit);
	layout->addWidget(_submitButton);
	layout->addWidget(_prioritySelect);
	setLayout(layout);
}

void FilterFrame::selectPriority() {
	int index = _prioritySelect->currentIndex();
	if (index == 0) {
		emit update({});
		return;
	}
	emit update({ "-p", std::to_string(index - 1).c_str() });
}

void FilterFrame::selectSinceDate() {
	QDialog dialog(this);
	dialog.setWindowTitle("Select Since Date");

	auto* dateTimeEdit = new QDateTimeEdit(QDateTime::currentDateTime(), &dialog);
	dateTimeEdit->setCalendarPopup(true);
	dateTimeEdit->setDisplayFormat("yyyy-MM-dd HH:mm:ss");

	auto* buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);

	auto* layout = new QVBoxLayout();
	layout->addWidget(dateTimeEdit);
	layout->addWidget(buttons);
	dialog.setLayout(layout);

	connect(buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
	connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

	if (dialog.exec() == QDialog::Accepted) {
		QString since = dateTimeEdit->dateTime().toString(Qt::ISODate);
		qDebug() << "Since date selected:" << since;
		const QStringList cmdList = { "--since", since };
		emit sinceDateSelected(since);
		emit update(cmdList);
	}
}

void FilterFrame::selectUntilDate() {
	QDialog dialog(this);
	dialog.setWindowTitle("Select Until Date");

	auto* dateTimeEdit = new QDateTimeEdit(QDateTime::currentDateTime(), &dialog);
	dateTimeEdit->setCalendarPopup(true);
	dateTimeEdit->setDisplayFormat("yyyy-MM-dd HH:mm:ss");

	auto* buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);

	auto* layout = new QVBoxLayout();
	layout->addWidget(dateTimeEdit);
	layout->addWidget(buttons);
	dialog.setLayout(layout);

	connect(buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
	connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

	if (dialog.exec() == QDialog::Accepted) {
		QString until = dateTimeEdit->dateTime().toString(Qt::ISODate);
		qDebug() << "Until date selected:" << until;
		const QStringList cmdList = { "--until", until };
		emit untilDateSelected(until);
		emit update(cmdList);
	}
}

void FilterFrame::submitFilterString() {
	const auto pattern = _stringEdit->text();
	if (pattern.isEmpty()) {
		emit update({});
		return;
	}
	const QStringList cmdList = { "-g", pattern };
	emit update(cmdList);
}


FilterFrame::~FilterFrame() = default;