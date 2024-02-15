#pragma once

#include <QtGui/QColor>
#include <QtWidgets/QPushButton>

namespace eno {

class ColorButton : public QPushButton {
	Q_OBJECT

public:
	ColorButton(QWidget* parent = 0);
	virtual ~ColorButton() = default;

	// clang-format off
	const QColor& color() const { return _color; }
	void setColor(const QColor& color);
	// clang-format on

private slots:
	void chooseColor();

private:
	using QPushButton::QPushButton;
	using QPushButton::setIcon;
	using QPushButton::setIconSize;
	using QPushButton::setText;
	using QPushButton::setToolTip;

	void updateColor();

	QColor _color{};

signals:
	void currentColorChanged(const QColor&);
};
} // namespace eno
