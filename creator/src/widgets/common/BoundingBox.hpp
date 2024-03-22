#pragma once

#include <QtWidgets/QSpinBox>

namespace eno {

class BoundingBox : public QSpinBox {
	Q_OBJECT

public:
	BoundingBox(QWidget* parent = 0);
	virtual ~BoundingBox() = default;

	void init(bool positive);
};
} // namespace eno
