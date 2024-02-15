#pragma once

#include <QtWidgets/QWidget>

class QLabel;

namespace eno {
class Scene;
class SpinBox;
class GraphicsView;

class InfoWidget : public QWidget {
	Q_OBJECT

public:
	InfoWidget(QWidget* parent = 0);
	virtual ~InfoWidget() = default;

	void init(Scene* scene, GraphicsView* graphicsView);

private slots:
	void updateData();

private:

	Scene* _scene{ nullptr };
	GraphicsView* _graphicsView{ nullptr };

	QLabel* _labelPosition{ nullptr };
	SpinBox* _minXSpinBox{ nullptr };
	SpinBox* _minYSpinBox{ nullptr };
	SpinBox* _maxXSpinBox{ nullptr };
	SpinBox* _maxYSpinBox{ nullptr };
};
} // namespace eno
