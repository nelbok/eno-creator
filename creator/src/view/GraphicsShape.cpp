#include "GraphicsShape.hpp"

#include <QtCore/QCoreApplication>
#include <QtGui/QPainter>

#include <eno/data/Material.hpp>
#include <eno/data/Object.hpp>
#include <eno/data/Project.hpp>
#include <eno/data/Scene.hpp>

#include "controller/Core.hpp"
#include "controller/Graphics.hpp"

namespace eno {
GraphicsShape::GraphicsShape(Core* core, QGraphicsItem* parent)
	: QGraphicsItem(parent)
	, _core(core) {}

void GraphicsShape::setMode(Mode mode) {
	_mode = mode;
}

QRectF GraphicsShape::boundingRect() const {
	const auto* scene = _core->project()->scene();
	const auto min = scene->min() * 10;
	const auto max = scene->max() * 10;
	return { min, max };
}

void GraphicsShape::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) {
	// Avoid scene graph update while loading a file
	if (_core->project()->thread() != QCoreApplication::instance()->thread()) {
		return;
	}

	QPen pen;
	QBrush brush{ Qt::SolidPattern };
	if (_mode == Mode::Below) {
		pen.setStyle(Qt::DotLine);
		brush.setStyle(Qt::Dense4Pattern);
	}
	painter->setPen(pen);

	auto depth = _core->graphics()->depth();
	const auto& objects = _core->project()->scene()->objects();
	for (auto* object : objects) {
		const auto& pos = object->position();
		if ((pos.y() == depth && _mode == Mode::Normal) || (pos.y() == depth - 1 && _mode == Mode::Below)) {
			brush.setColor(object->material()->diffuse());
			painter->setBrush(brush);
			painter->drawRect(pos.x() * 10, pos.z() * 10, 9, 9);
		}
	}
}
} // namespace eno
