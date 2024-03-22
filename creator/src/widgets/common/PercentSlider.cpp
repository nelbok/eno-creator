#include "PercentSlider.hpp"

#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QSlider>

namespace eno {
PercentSlider::PercentSlider(QWidget* parent)
	: QWidget(parent) {}

void PercentSlider::init() {
	auto* l = new QHBoxLayout(this);
	l->setContentsMargins({ 0, 0, 0, 0 });

	_sl = new QSlider(this);
	_sp = new QDoubleSpinBox(this);

	l->addWidget(_sl);
	l->addWidget(_sp);

	_sl->setRange(0, 100);
	_sl->setSingleStep(10);
	_sl->setOrientation(Qt::Orientation::Horizontal);
	connect(_sl, &QSlider::valueChanged, this, [this](int value) {
		setValue(value / 100.0);
	});

	_sp->setDecimals(2);
	_sp->setRange(0.0, 1.0);
	_sp->setSingleStep(0.1);
	connect(_sp, &QDoubleSpinBox::valueChanged, this, [this](double value) {
		setValue(value);
	});
}

void PercentSlider::setValue(double value) {
	if (_sp->value() == value) {
		return;
	}

	QSignalBlocker ssp(_sp);
	QSignalBlocker ssl(_sl);

	_sp->setValue(value);
	_sl->setValue(static_cast<int>(value * 100.0));

	emit valueChanged(value);
}
} // namespace eno
