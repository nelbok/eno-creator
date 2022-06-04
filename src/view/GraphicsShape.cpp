#include "GraphicsShape.hpp"

#include <QPainter>

#include "controller/MapAction.hpp"
#include "data/Project.hpp"
#include "data/Material.hpp"
#include "data/Scene.hpp"

namespace eno {
GraphicsShape::GraphicsShape(MapAction* mapAction, QGraphicsItem* parent)
	: QGraphicsItem(parent)
	, _mapAction(mapAction) {}

void GraphicsShape::setMode(Mode mode) {
	_mode = mode;
}

QRectF GraphicsShape::boundingRect() const {
	const auto* scene = _mapAction->project()->scene();
	const auto min = scene->min() * 10;
	const auto max = scene->max() * 10;
	return { min, max };
}

void GraphicsShape::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) {
	QPen pen;
	QBrush brush{ Qt::SolidPattern };
	if (_mode == Mode::Below) {
		pen.setStyle(Qt::DotLine);
		brush.setStyle(Qt::Dense4Pattern);
	}
	painter->setPen(pen);
	for (const auto& pair : *(_mapAction->project()->scene())) {
		const auto& pos = pair.first;
		if ((pos.y() == _mapAction->depth() && _mode == Mode::Normal) || (pos.y() == _mapAction->depth() - 1 && _mode == Mode::Below)) {
			brush.setColor(pair.second->diffuse());
			painter->setBrush(brush);
			painter->drawRect(pos.x() * 10, pos.z() * 10, 9, 9);
		}
	}
}
} // namespace eno
