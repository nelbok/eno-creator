#pragma once

#include <QtWidgets/QWidget>

class QSlider;
class QDoubleSpinBox;

namespace eno {

class PercentSlider : public QWidget {
	Q_OBJECT

public:
	PercentSlider(QWidget* parent = 0);
	virtual ~PercentSlider() = default;

	void init();

	// Value [0, 1.0]
	void setValue(double value);

private:
	QSlider* _sl{ nullptr };
	QDoubleSpinBox* _sp{ nullptr };

signals:
	// Value [0, 1.0]
	void valueChanged(double value);
};
} // namespace eno
