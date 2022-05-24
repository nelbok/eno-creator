#pragma once

#include <QGraphicsItem>
#include <QColor>

class QVector3D;

namespace eno {
class MapAction;

class GraphicsItem : public QGraphicsItem {
public:
	GraphicsItem(MapAction* mapAction, QGraphicsItem* parent = 0);

	void setPos(const QVector3D& pos);
	void setColor(const QColor& color);

	virtual QRectF boundingRect() const;
	virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0);

private:
	MapAction* _mapAction{nullptr};
	QColor _color{};
};
} // namespace srp_creator
