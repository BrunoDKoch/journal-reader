#include "widgets/boot_widget.hpp"

BootWidget::BootWidget(QWidget* parent, int boot) :
	QFrame(parent),
	_boot(boot) {
	_numberDisplay = new QSpinBox(this);
	_numberDisplay->setRange(0, 100);
	_numberDisplay->setSingleStep(1);
	_numberDisplay->setPrefix("Boot ");
	_numberDisplay->setValue(_boot);
	connect(_numberDisplay, &QSpinBox::valueChanged, this, &BootWidget::setBoot);
	auto layout = new QHBoxLayout();
	layout->addWidget(_numberDisplay);
	setLayout(layout);
}

void BootWidget::setBoot() {
	_boot = _numberDisplay->value();
	emit bootSet(_boot);
}