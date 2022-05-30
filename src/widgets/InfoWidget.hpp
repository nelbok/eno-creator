#pragma once

#include <QWidget>

class QLabel;
class QSpinBox;
class QHBoxLayout;

namespace eno {
class Data;
class GraphicsView;

class InfoWidget : public QWidget {
	Q_OBJECT

public:
	InfoWidget(QWidget* parent = 0);
	virtual ~InfoWidget() = default;

	void init(Data* data, GraphicsView* graphicsView);

private slots:
	void updateData();

private:
	QSpinBox* createSpinBox(bool positive);

	Data* _data{ nullptr };
	GraphicsView* _graphicsView{ nullptr };

	QLabel* _labelPosition{ nullptr };
	QSpinBox* _minXSpinBox{ nullptr };
	QSpinBox* _minYSpinBox{ nullptr };
	QSpinBox* _maxXSpinBox{ nullptr };
	QSpinBox* _maxYSpinBox{ nullptr };
};
} // namespace eno
