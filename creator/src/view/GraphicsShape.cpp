#include "GraphicsShape.hpp"

#include <QtCore/QCoreApplication>
#include <QtGui/QPainter>

#include <eno/data/Material.hpp>
#include <eno/data/Object.hpp>
#include <eno/data/Project.hpp>
#include <eno/data/Scene.hpp>

#include "controller/MapAction.hpp"

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
	// Avoid scene graph update while loading a file
	if (_mapAction->project()->thread() != QCoreApplication::instance()->thread()) {
		return;
	}

	QPen pen;
	QBrush brush{ Qt::SolidPattern };
	if (_mode == Mode::Below) {
		pen.setStyle(Qt::DotLine);
		brush.setStyle(Qt::Dense4Pattern);
	}
	painter->setPen(pen);
	for (auto* object : _mapAction->project()->scene()->objects()) {
		const auto& pos = object->position();
		if ((pos.y() == _mapAction->depth() && _mode == Mode::Normal) || (pos.y() == _mapAction->depth() - 1 && _mode == Mode::Below)) {
			brush.setColor(object->material()->diffuse());
			painter->setBrush(brush);
			painter->drawRect(pos.x() * 10, pos.z() * 10, 9, 9);
		}
	}
}
} // namespace eno
