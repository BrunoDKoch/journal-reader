#pragma once
#include <QtWidgets>
#include "model/journal_model.hpp"

class DetailsView : public QDialog {
	Q_OBJECT
public:
	explicit DetailsView(const QIcon& icon, const QString& data, const QString& details, QWidget* parent = nullptr);
	~DetailsView();

private:
	QVBoxLayout _layout;
	QScrollArea* _detailsArea;
	QTextBrowser* _textBrowser;
};