#pragma once

#include <QGraphicsRectItem>
#include <QGraphicsLineItem>
#include <QGraphicsView>

namespace eno {
class MapAction;
class GraphicsItem;

class GraphicsView : public QGraphicsView {
	Q_OBJECT

public:
	GraphicsView(MapAction* mapAction, QWidget* parent = 0);
	virtual ~GraphicsView() = default;

	void init();

protected:
	virtual void mousePressEvent(QMouseEvent* e);
	virtual void mouseMoveEvent(QMouseEvent* e);

private slots:
	void updateMap();
	void updateRect();
	void updateZoom();

private:
	const QVector2D mapToData(const QPoint& pos) const;

	MapAction* _mapAction{ nullptr };

	QGraphicsScene* _scene{ nullptr };
	QGraphicsRectItem _rect{};
	QGraphicsLineItem _xAxis{};
	QGraphicsLineItem _yAxis{};

	QMap<QVector3D, GraphicsItem*> _items{};
};
} // namespace eno
