#pragma once

#include <QtWidgets/QPushButton>

namespace eno {

class KeyButton : public QPushButton {
	Q_OBJECT

public:
	KeyButton(QWidget* parent = 0);
	virtual ~KeyButton();

	// clang-format off
	Qt::Key key() const { return _key; }
	void setKey(Qt::Key key);
	// clang-format on

protected:
	virtual void keyReleaseEvent(QKeyEvent* event);

private slots:
	void process();

private:
	using QPushButton::QPushButton;
	using QPushButton::setIcon;
	using QPushButton::setIconSize;
	using QPushButton::setText;
	using QPushButton::setToolTip;

	void checkConflicts();
	void updateText();

	Qt::Key _key{};
	bool _edit{ false };

signals:
	void currentKeyChanged(Qt::Key);
};
} // namespace eno
