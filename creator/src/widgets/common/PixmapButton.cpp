#include "PixmapButton.hpp"

#include <QtWidgets/QApplication>
#include <QtWidgets/QFileDialog>

#include "controller/Preferences.hpp"

namespace eno {
PixmapButton::PixmapButton(QWidget* parent)
	: QPushButton(parent) {
	setFixedSize(68, 68);
	setIconSize({ 64, 64 });
	connect(this, &QPushButton::clicked, this, &PixmapButton::choosePixmap);
}

void PixmapButton::setPixmap(const QPixmap& pixmap) {
	_pixmap = pixmap;
	updatePixmap();
}

void PixmapButton::choosePixmap() {
	auto fileName = QFileDialog::getOpenFileName(qApp->activeWindow(), qApp->applicationName() + " - Open", Preferences::projectLocation(), "Image Files (*.gif *.png *.jpg *.bmp)");
	if (!fileName.isEmpty()) {
		QPixmap pixmap;
		bool success = pixmap.load(fileName);
		if (success) {
			_pixmap = pixmap;
			updatePixmap();
			emit currentPixmapChanged(_pixmap);
		}
	}
}

void PixmapButton::updatePixmap() {
	setIcon({ _pixmap });
}

} // namespace eno
