#include "widgets/page_widget.hpp"

PageWidget::PageWidget(QWidget* parent, int page, int numberOfDisplayedItems) :
	QFrame(parent),
	_page(page),
	_numberOfDisplayedItems(numberOfDisplayedItems) {
	_numberOfItemsBox = new QComboBox(this);
	for (auto& option : _displayedItemsOptions) {
		_numberOfItemsBox->addItem(std::to_string(option).c_str());
	}
	connect(_numberOfItemsBox, &QComboBox::currentIndexChanged, this, &PageWidget::setNumberOfDisplayedItems);
	_previousPageButton = new QPushButton("Previous");
	connect(_previousPageButton, &QPushButton::clicked, this, &PageWidget::setPreviousPage);
	_nextPageButton = new QPushButton("Next");
	connect(_nextPageButton, &QPushButton::clicked, this, &PageWidget::setNextPage);
	_previousPageButton->setDisabled(true);
	auto layout = new QHBoxLayout();
	layout->addWidget(_previousPageButton);
	layout->addWidget(_numberOfItemsBox);
	layout->addWidget(_nextPageButton);
	setLayout(layout);
}

void PageWidget::setNumberOfDisplayedItems() {
	emit numberOfDisplayedItemsSet(_displayedItemsOptions[_numberOfItemsBox->currentIndex()]);
}

void PageWidget::setPage(bool isPrevious) {
	if (_page <= 1 && isPrevious) return;
	if (isPrevious)	_page--;
	else _page++;
	_previousPageButton->setDisabled(_page <= 1);
	emit pageSet(_page);
}

void PageWidget::setPreviousPage() {
	setPage(true);
}

void PageWidget::setNextPage() {
	setPage(false);
}