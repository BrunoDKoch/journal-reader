#pragma once

#include "structs/journal_entry.hpp"
#include "enums/priority_enum.hpp"
#include <QAbstractTableModel>
#include <QIcon>
#include <QStyle>
#include <QApplication>

class JournalModel : public QAbstractTableModel {
	Q_OBJECT
public:
	explicit JournalModel(QObject* parent = nullptr);

	int rowCount(const QModelIndex& parent = QModelIndex()) const override;
	int columnCount(const QModelIndex& parent = QModelIndex()) const override;
	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
	QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

	void appendEntry(const JournalEntry& entry);
	static QIcon getIcon(const QString& priorityString);
	void clear();

	QString getOldestCursor();
	QString getNewestCursor();

	QString getFullData(const QModelIndex& index);
	QString getExtraDetails(const QModelIndex& index);
	QString getAsExportLogEntry(const QModelIndex& index);

	QIcon revealIcon(const QModelIndex& index);

private:
	QVector<JournalEntry> _entries;
};

