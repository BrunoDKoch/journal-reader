#pragma once
#include <QtWidgets>
#include <QShortcut>
#include <QKeySequence>
#include "model/journal_model.hpp"
#include "widgets/filter_frame.hpp"
#include "widgets/page_widget.hpp"
#include "widgets/details_view.hpp"
#include "widgets/boot_widget.hpp"

class MainFrame : public QFrame {
	Q_OBJECT
public:
	explicit MainFrame(QWidget* parent = nullptr);
	~MainFrame();

public slots:
	void reload();
	void exportLogs();
	void copySelectedLogs();

private slots:
	void readOutput();
	void setFilterStringList(const QStringList& list);
	void updateNumberOfItems(int numberOfItems);
	void showDetails(const QModelIndex& index);
	void updateBoot(int boot);
	void toggleProgressVisibility();
	void onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);
	const QString getAllCurrentLogEntries();

private:
	QTableView* _table;
	JournalModel* _model;
	QProcess* _process;
	QStringList _filterStringList;
	QStringList _defaultArgs;
	PageWidget* _pageWidget;
	BootWidget* _bootWidget;
	QProgressBar* _progressBar;
	QFrame* _progressFrame;
	int _page = 1;
	int _numberOfItems = 50;
	int _boot = 0;
};