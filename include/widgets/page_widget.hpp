#pragma once
#include <QtWidgets>

class PageWidget : public QFrame {
	Q_OBJECT
public:
	explicit PageWidget(QWidget* parent = nullptr, int page = 1, int numberOfDisplayedItems = 50);
	~PageWidget() = default;

signals:
	void pageSet(int number);
	void numberOfDisplayedItemsSet(int number);

private slots:
	void setPreviousPage();
	void setNextPage();
	void setPage(bool isPrevious);
	void setNumberOfDisplayedItems();

private:
	int _page = 1;
	int _numberOfDisplayedItems = 50;
	QComboBox* _numberOfItemsBox;
	int _displayedItemsOptions[4] = { 50, 100, 200, 500 };
	QPushButton* _previousPageButton;
	QPushButton* _nextPageButton;
};