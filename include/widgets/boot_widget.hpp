#pragma once
#include <QtWidgets>

class BootWidget : public QFrame {
	Q_OBJECT
public:
	explicit BootWidget(QWidget* parent = nullptr, int boot = 0);
	~BootWidget() = default;

signals:
	void bootSet(int boot);

private slots:
	void setBoot();

private:
	int _boot = 0;
	QSpinBox* _numberDisplay;
};