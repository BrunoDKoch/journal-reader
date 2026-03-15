
#pragma once
#include <QtCore>

struct JournalEntry {
	QString timestamp;
	QString message;
	QString hostname;
	QString unit;
	QString priority;
	QString cursor;
	QMap<QString, QString> extra;
};
