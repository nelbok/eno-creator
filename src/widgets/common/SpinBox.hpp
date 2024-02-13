#pragma once

#include <QSpinBox>

namespace eno {

class SpinBox : public QSpinBox {
	Q_OBJECT

public:
	SpinBox(QWidget* parent = 0);
	virtual ~SpinBox() = default;

	void init(bool positive);
};
} // namespace eno
