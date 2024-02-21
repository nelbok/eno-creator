#include "GraphicsView.hpp"

#include <cmath>

#include <QtGui/QMouseEvent>
#include <QtWidgets/QGraphicsRectItem>
#include <QtWidgets/QGraphicsLineItem>

#include <eno/data/Project.hpp>
#include <eno/data/Scene.hpp>

#include "controller/Core.hpp"
#include "controller/Graphics.hpp"
#include "controller/UndoRedo.hpp"
#include "GraphicsShape.hpp"

namespace eno {

GraphicsView::GraphicsView(Core* core, QWidget* parent)
	: QGraphicsView(parent)
	, _core(core)
	, _graphics(core->graphics()) {}

void GraphicsView::init() {
	setMouseTracking(true);

	_rect = new QGraphicsRectItem;
	_xAxis = new QGraphicsLineItem;
	_yAxis = new QGraphicsLineItem;

	QPen pen;
	pen.setStyle(Qt::DashLine);
	_xAxis->setPen(pen);
	_yAxis->setPen(pen);

	_shapeNormal = new GraphicsShape(_core);
	_shapeBelow = new GraphicsShape(_core);
	_shapeBelow->setMode(GraphicsShape::Mode::Below);

	// We need to respect the order for the paint event
	_scene = new QGraphicsScene(this);
	_scene->addItem(_shapeBelow);
	_scene->addItem(_shapeNormal);
	_scene->addItem(_rect);
	_scene->addItem(_xAxis);
	_scene->addItem(_yAxis);
	setScene(_scene);

	// FIXME: We have now undo redo maybe we don't need this two connections
	connect(_core->project()->scene(), &Scene::objectsUpdated, this, &GraphicsView::updateShapes);
	connect(_core->project()->scene(), &Scene::rectUpdated, this, &GraphicsView::updateRect);
	connect(_core->undoRedo(), &UndoRedo::updated, this, &GraphicsView::updateShapes);
	connect(_graphics, &Graphics::depthUpdated, this, &GraphicsView::updateShapes);
	connect(_graphics, &Graphics::zoomUpdated, this, &GraphicsView::updateZoom);
}

const QVector2D& GraphicsView::pointerPosition() const {
	return _pointerPosition;
}

void GraphicsView::mousePressEvent(QMouseEvent* e) {
	QGraphicsView::mousePressEvent(e);
	if (e->buttons() == Qt::LeftButton) {
		_graphics->mousePressEvent(mapToData(e->pos()));
	}
}

void GraphicsView::mouseMoveEvent(QMouseEvent* e) {
	QGraphicsView::mouseMoveEvent(e);
	const auto& pos = mapToData(e->pos());
	_pointerPosition = { pos.x(), pos.z() };
	if (e->buttons() == Qt::LeftButton) {
		_graphics->mouseMoveEvent(pos);
	}
	emit pointerPositionUpdated();
	setCursor(_graphics->cursorShape());
}

void GraphicsView::updateShapes() {
	_shapeNormal->update();
	_shapeBelow->update();
}

void GraphicsView::updateRect() {
	const auto* scene = _core->project()->scene();
	const auto min = scene->min() * 10;
	const auto max = scene->max() * 10;

	const QRectF rect{ min, max };
	_scene->setSceneRect(rect);
	_rect->setRect(rect);
	_xAxis->setLine(min.x() - 30.f, 0.f, max.x() + 30.f, 0.f);
	_yAxis->setLine(0, min.y() - 30.f, 0, max.y() + 30.f);
}

void GraphicsView::updateZoom() {
	auto transform = QTransform{};
	auto zoom = std::underlying_type_t<Preferences::Zoom>(_graphics->zoom()) / 100.0;
	transform.scale(zoom, zoom);
	setTransform(transform);
}

const QVector3D GraphicsView::mapToData(const QPoint& pos) const {
	const auto& posF = mapToScene(pos);
	return { floorf(posF.x() / 10.f), floorf(_graphics->depth()), floorf(posF.y() / 10.f) };
}

} // namespace eno
