#include "GraphicsItem.hpp"

#include <QPainter>
#include <QVector3D>

#include "controller/MapAction.hpp"

namespace eno {
GraphicsItem::GraphicsItem(MapAction* mapAction, QGraphicsItem* parent)
	: QGraphicsItem(parent)
	, _mapAction(mapAction) {}

void GraphicsItem::setPos(const QVector3D& pos) {
	QGraphicsItem::setPos({ pos.x() * 10.f, pos.y() * 10.f });
	QGraphicsItem::setZValue(pos.z());
}

void GraphicsItem::setColor(const QColor& color) {
	_color = color;
}

QRectF GraphicsItem::boundingRect() const {
	return QRectF(0.f, 0.f, 9.f, 9.f);
}

void GraphicsItem::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) {
	QPen pen;
	QBrush brush(_color);

	if (zValue() < _mapAction->depth()) {
		pen.setStyle(Qt::DotLine);
		brush.setStyle(Qt::Dense4Pattern);
	}

	painter->setPen(pen);
	painter->setBrush(brush);
	painter->drawRect(0, 0, 9, 9);
}
} // namespace eno
