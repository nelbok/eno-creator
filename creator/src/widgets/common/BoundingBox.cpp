#include "BoundingBox.hpp"


namespace eno {
BoundingBox::BoundingBox(QWidget* parent)
	: QSpinBox(parent) {}

void BoundingBox::init(bool positive) {
	setFixedWidth(60);
	if (positive) {
		setRange(5, 999);
	} else {
		setRange(-999, -5);
	}
}

} // namespace eno
