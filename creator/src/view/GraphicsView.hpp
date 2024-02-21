#pragma once

#include <QtGui/QVector2D>
#include <QtWidgets/QGraphicsView>

class QGraphicsRectItem;
class QGraphicsLineItem;

namespace eno {
class Core;
class Graphics;
class GraphicsShape;

class GraphicsView : public QGraphicsView {
	Q_OBJECT

public:
	GraphicsView(Core* core, QWidget* parent = 0);
	virtual ~GraphicsView() = default;

	void init();
	const QVector2D& pointerPosition() const;

protected:
	virtual void mousePressEvent(QMouseEvent* e);
	virtual void mouseMoveEvent(QMouseEvent* e);

private slots:
	void updateShapes();
	void updateRect();
	void updateZoom();

private:
	const QVector3D mapToData(const QPoint& pos) const;

	Core* _core{ nullptr };
	Graphics* _graphics{ nullptr };
	QVector2D _pointerPosition{};

	QGraphicsScene* _scene{ nullptr };
	QGraphicsRectItem* _rect{ nullptr };
	QGraphicsLineItem* _xAxis{ nullptr };
	QGraphicsLineItem* _yAxis{ nullptr };
	GraphicsShape* _shapeNormal{ nullptr };
	GraphicsShape* _shapeBelow{ nullptr };

signals:
	void pointerPositionUpdated();
};
} // namespace eno
