#pragma once
#include <QtWidgets>
#include "structs/journal_model.hpp"

class DetailsView : public QDialog {
	Q_OBJECT
public:
	explicit DetailsView(const QString& data, const QString& details, QWidget* parent = nullptr);
	~DetailsView();

private:
	QVBoxLayout _layout;
	QToolButton _toggleButton;
	QParallelAnimationGroup _toggleAnimation;
	QScrollArea _detailsArea;
};