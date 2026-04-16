#include "widgets/details_view.hpp"

DetailsView::DetailsView(const QIcon& icon, const QString& data, const QString& details, QWidget* parent) {
	setWindowIcon(icon);
	setWindowTitle("Detailed event information");
	QFrame* detailsFrame = new QFrame(this);

	_textBrowser = new QTextBrowser(detailsFrame);
	_textBrowser->setText(data + "\n\nDETAILS:\n" + details);

	_textBrowser->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
	_layout.addWidget(_textBrowser);

	this->setLayout(&_layout);
}

DetailsView::~DetailsView() = default;