#include "KeyButton.hpp"

#include <QtGui/QKeyEvent>

namespace eno {

namespace detail {
QList<KeyButton*>& instances() {
	static QList<KeyButton*> s_instances{};
	return s_instances;
}
} // namespace detail

KeyButton::KeyButton(QWidget* parent)
	: QPushButton(parent) {
	connect(this, &QPushButton::clicked, this, &KeyButton::process);
	setFixedWidth(100);
	detail::instances().append(this);
}

KeyButton::~KeyButton() {
	detail::instances().removeAll(this);
}

void KeyButton::setKey(Qt::Key key) {
	if (key != _key) {
		_key = key;
		checkConflicts();
		updateText();
	}
}

void KeyButton::keyReleaseEvent(QKeyEvent* e) {
	if (_edit) {
		_key = static_cast<Qt::Key>(e->key());
		checkConflicts();
		process();
	} else {
		QPushButton::keyReleaseEvent(e);
	}
}

void KeyButton::process() {
	if (_edit) {
		releaseKeyboard();
		_edit = false;
	} else {
		// Release other instances
		for (auto* instance : detail::instances()) {
			if (instance != this && instance->_edit) {
				instance->process();
			}
		}
		grabKeyboard();
		_edit = true;
	}
	updateText();
}

void KeyButton::checkConflicts() {
	for (auto* instA : detail::instances()) {
		QPalette palette = instA->palette();
		palette.setColor(QPalette::ButtonText, QColorConstants::Black);
		for (auto* instB : detail::instances()) {
			if (instA != instB && instA->_key == instB->_key) {
				palette.setColor(QPalette::ButtonText, QColorConstants::Red);
				break;
			}
		}
		instA->setPalette(palette);
	}
}

void KeyButton::updateText() {
	if (_edit) {
		setText("[Press a key]");
	} else {
		QString text = QKeySequence(_key).toString();
		// Work around: Special case to bypass sequence auto mnemonic
		if (text == "&") {
			text = "&&";
		}
		setText(text);
	}
}
} // namespace eno
