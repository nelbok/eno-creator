#include "SpinBox.hpp"


namespace eno {
SpinBox::SpinBox(QWidget* parent)
	: QSpinBox(parent) {}

void SpinBox::init(bool positive) {
	setFixedWidth(60);
	if (positive) {
		setRange(5, 999);
	} else {
		setRange(-999, -5);
	}
}

} // namespace eno
