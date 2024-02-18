#include "ColorButton.hpp"

#include <QtWidgets/QApplication>
#include <QtWidgets/QColorDialog>

namespace eno {
ColorButton::ColorButton(QWidget* parent)
	: QPushButton(parent) {
	connect(this, &QPushButton::clicked, this, &ColorButton::chooseColor);
}

void ColorButton::setColor(const QColor& color) {
	if (color != _color) {
		_color = color;
		updateColor();
	}
}

void ColorButton::chooseColor() {
	auto color = QColorDialog::getColor(_color, qApp->activeWindow(), qApp->applicationName() + " - Open");
	if (color != _color && color.isValid()) {
		_color = color;
		updateColor();
		emit currentColorChanged(_color);
	}
}

void ColorButton::updateColor() {
	setText(_color.name());

	auto image = QImage(48, 48, QImage::Format_RGB32);
	image.fill(_color);
	setIcon({ QPixmap::fromImage(image) });
}

} // namespace eno
