#pragma once

#include <QWidget>

class QLabel;
class QSpinBox;
class QHBoxLayout;

namespace eno {
class Scene;
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
	QSpinBox* createSpinBox(bool positive);

	Scene* _scene{ nullptr };
	GraphicsView* _graphicsView{ nullptr };

	QLabel* _labelPosition{ nullptr };
	QSpinBox* _minXSpinBox{ nullptr };
	QSpinBox* _minYSpinBox{ nullptr };
	QSpinBox* _maxXSpinBox{ nullptr };
	QSpinBox* _maxYSpinBox{ nullptr };
};
} // namespace eno
