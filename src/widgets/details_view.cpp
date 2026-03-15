#include "widgets/details_view.hpp"

DetailsView::DetailsView(const QString& data, const QString& details, QWidget* parent) {
	QFrame* detailsFrame = new QFrame(this);
	QLabel* label = new QLabel(data, detailsFrame);
	label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
	label->setWordWrap(true);
	_layout.addWidget(label);

	QLabel* detailsLabel = new QLabel(details, detailsFrame);
	detailsLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
	detailsLabel->setWordWrap(true);
	_layout.addWidget(detailsLabel);

	this->setLayout(&_layout);
}

DetailsView::~DetailsView() = default;