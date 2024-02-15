#pragma once

#include <QtWidgets/QWidget>

class QSpinBox;

namespace eno {

class SpinBoxTool : public QWidget {
	Q_OBJECT

public:
	SpinBoxTool(QWidget* parent = 0);
	virtual ~SpinBoxTool() = default;

	void init(const QString& icon);
	void setRange(int min, int max);

public slots:
	void setValue(int value);

private:
	QSpinBox* _spinBox{ nullptr };

signals:
	void valueChanged(int);
};
} // namespace eno
