#include "SpinBoxTool.hpp"

#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpinBox>

namespace eno {
SpinBoxTool::SpinBoxTool(QWidget* parent)
	: QWidget(parent) {}

void SpinBoxTool::init(const QString& icon) {
	_spinBox = new QSpinBox(this);
	auto* label = new QLabel(this);

	label->setPixmap(QPixmap(icon));

	QHBoxLayout* layout = new QHBoxLayout(this);
	layout->addWidget(_spinBox);
	layout->addWidget(label);

	connect(_spinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, [this](int value) {
		emit valueChanged(value);
	});
}

void SpinBoxTool::setRange(int min, int max) {
	assert(_spinBox);
	assert(min < max);
	_spinBox->setRange(min, max);
}

void SpinBoxTool::setValue(int value) {
	assert(_spinBox->minimum() <= value && value <= _spinBox->maximum());
	_spinBox->setValue(value);
}

} // namespace eno
