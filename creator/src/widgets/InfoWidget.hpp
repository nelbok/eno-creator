#pragma once

#include <QtWidgets/QWidget>

class QLabel;

namespace eno {
class BoundingBox;
class Commands;
class Scene;
class GraphicsView;

class InfoWidget : public QWidget {
	Q_OBJECT

public:
	InfoWidget(QWidget* parent = 0);
	virtual ~InfoWidget() = default;

	void init(Commands* commands, Scene* scene, GraphicsView* graphicsView);

private slots:
	void updateData();

private:

	Scene* _scene{ nullptr };
	GraphicsView* _graphicsView{ nullptr };

	QLabel* _labelPosition{ nullptr };
	BoundingBox* _minX{ nullptr };
	BoundingBox* _minY{ nullptr };
	BoundingBox* _maxX{ nullptr };
	BoundingBox* _maxY{ nullptr };
};
} // namespace eno
