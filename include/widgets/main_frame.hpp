#pragma once
#include <QtWidgets>
#include "structs/journal_model.hpp"
#include "widgets/filter_frame.hpp"
#include "widgets/page_widget.hpp"
#include "widgets/details_view.hpp"
#include "widgets/boot_widget.hpp"

class MainFrame : public QFrame {
	Q_OBJECT
public:
	explicit MainFrame(QWidget* parent = nullptr);
	~MainFrame();

private slots:
	void readOutput();
	void reload();
	void setFilterStringList(const QStringList& list);
	void updateCursor(int page);
	void updateNumberOfItems(int numberOfItems);
	void showDetails(const QModelIndex& index);
	void updateBoot(int boot);
	void toggleProgressVisibility();
	void onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);

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
	QString _oldestCursor;
	QString _newestCursor;
	bool _moveToNextPage = false;
	bool _moveToPreviousPage = false;
};