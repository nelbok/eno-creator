#include "GraphicsShape.hpp"

#include <QPainter>

#include "data/Data.hpp"
#include "controller/MapAction.hpp"

namespace eno {
GraphicsShape::GraphicsShape(MapAction* mapAction, QGraphicsItem* parent)
	: QGraphicsItem(parent)
	, _mapAction(mapAction) {}

void GraphicsShape::setMode(Mode mode) {
	_mode = mode;
}

QRectF GraphicsShape::boundingRect() const {
	const auto& min = _mapAction->data()->min() * 10.f;
	const auto& max = _mapAction->data()->max() * 10.f;
	return { min.x(), min.y(), max.x() - min.x(), max.y() - min.y() };
}

void GraphicsShape::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) {
	QPen pen;
	QBrush brush{ Qt::SolidPattern };
	if (_mode == Mode::Below) {
		pen.setStyle(Qt::DotLine);
		brush.setStyle(Qt::Dense4Pattern);
	}
	painter->setPen(pen);
	for (const auto& pair : *(_mapAction->data())) {
		const auto& pos = pair.first;
		if ((pos.z() == _mapAction->depth() && _mode == Mode::Normal) || (pos.z() == _mapAction->depth() - 1 && _mode == Mode::Below)) {
			brush.setColor(pair.second);
			painter->setBrush(brush);
			painter->drawRect(pos.x() * 10.f, pos.y() * 10.f, 9, 9);
		}
	}
}
} // namespace eno
