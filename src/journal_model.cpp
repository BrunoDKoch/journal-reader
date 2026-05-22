#include "model/journal_model.hpp"

JournalModel::JournalModel(QObject* parent)
	: QAbstractTableModel(parent) {
}

int JournalModel::rowCount(const QModelIndex&) const {
	return _entries.size();
}

int JournalModel::columnCount(const QModelIndex&) const {
	return 5;
}

QVariant JournalModel::headerData(int section, Qt::Orientation orientation, int role) const {
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
		switch (section) {
			case 0: return "Time";
			case 1: return "Priority";
			case 2: return "Unit";
			case 3: return "Host";
			case 4: return "Message";
		}
	}
	return {};
}

void JournalModel::appendEntry(const JournalEntry& entry) {
	beginInsertRows(QModelIndex(), _entries.size(), _entries.size());
	_entries.prepend(entry);
	endInsertRows();
}

QIcon JournalModel::getIcon(const QString& priorityString) {
	const QMetaObject* metaObj = &PriorityEnum::staticMetaObject;
	int enumIndex = metaObj->indexOfEnumerator("Priority");
	QMetaEnum metaEnum = metaObj->enumerator(enumIndex);
	int value = metaEnum.keyToValue(priorityString.toUtf8().data());
	PriorityEnum::Priority priority = static_cast<PriorityEnum::Priority>(value);

	switch (priority) {
		case PriorityEnum::Priority::EMERGENCY:
		case PriorityEnum::Priority::ALERT:
		case PriorityEnum::Priority::CRITICAL:
			return QApplication::style()->standardIcon(QStyle::SP_MessageBoxCritical);
		case PriorityEnum::Priority::ERROR:
			return QApplication::style()->standardIcon(QStyle::SP_BrowserStop);
		case PriorityEnum::Priority::WARNING:
			return QApplication::style()->standardIcon(QStyle::SP_MessageBoxWarning);
		case PriorityEnum::Priority::NOTICE:
		case PriorityEnum::Priority::INFO:
			return QApplication::style()->standardIcon(QStyle::SP_MessageBoxInformation);
		case PriorityEnum::Priority::DEBUG:
		default:
			return QApplication::style()->standardIcon(QStyle::SP_FileDialogInfoView);
	}
}

QIcon JournalModel::revealIcon(const QModelIndex& index) {
	const auto& e = _entries[index.row()];
	return JournalModel::getIcon(e.priority);
}

QVariant JournalModel::data(const QModelIndex& index, int role) const {
	if (!index.isValid())
		return {};

	const auto& e = _entries[index.row()];

	if (role == Qt::DisplayRole) {
		switch (index.column()) {
			case 0: return e.timestamp;
			case 1: return e.priority;
			case 2: return e.unit;
			case 3: return e.hostname;
			case 4: return e.message;
		}
	}

	if (role == Qt::DecorationRole && index.column() == 1) {
		return getIcon(e.priority);
	}

	return {};
}

void JournalModel::clear() {
	if (_entries.isEmpty())
		return;

	beginResetModel();
	_entries.clear();
	endResetModel();
}

QString JournalModel::getOldestCursor() {
	return _entries.last().cursor;
}

QString JournalModel::getNewestCursor() {
	return _entries.first().cursor;
}

QString JournalModel::getFullData(const QModelIndex& index) {
	const auto& e = _entries[index.row()];
	QString fullData = "Timestamp: " + e.timestamp + "\n" + "Priority level: " + e.priority + "\n" + "Hostname: " + e.hostname + "\n" + "Unit: " + e.unit + "\n" + "Message: " + e.message + "\n" + "Cursor: " + e.cursor;
	return fullData;
}

QString JournalModel::getExtraDetails(const QModelIndex& index) {
	const auto& e = _entries[index.row()];
	QString details;
	if (e.extra.count()) details += "\n";
	for (auto [k, v] : e.extra.asKeyValueRange()) {
		details += k + " : " + v + "\n";
	}
	return details;
}

QString JournalModel::getAsExportLogEntry(const QModelIndex& index) {
	const auto& e = _entries[index.row()];
	QString result = QString("[%1] - [%2] - [%3 @ %4] - %5").arg(e.timestamp, e.priority, e.unit, e.hostname, e.message);
	QString details = getExtraDetails(index);
	if (!details.isEmpty()) {
		result += details;
	}
	return result;
}