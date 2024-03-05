#pragma once

#include <QtGui/QPixmap>
#include <QtWidgets/QPushButton>

namespace eno {

class PixmapButton : public QPushButton {
	Q_OBJECT

public:
	PixmapButton(QWidget* parent = 0);
	virtual ~PixmapButton() = default;

	// clang-format off
	const QPixmap& pixmap() const { return _pixmap; }
	void setPixmap(const QPixmap& pixmap);
	// clang-format on

private slots:
	void choosePixmap();

private:
	using QPushButton::QPushButton;
	using QPushButton::setIcon;
	using QPushButton::setIconSize;
	using QPushButton::setText;
	using QPushButton::setToolTip;

	void updatePixmap();

	QPixmap _pixmap{};

signals:
	void currentPixmapChanged(const QPixmap&);
};
} // namespace eno
