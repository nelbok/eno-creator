#include "GraphicsView.hpp"

#include <cmath>

#include <QGraphicsRectItem>
#include <QGraphicsLineItem>
#include <QMouseEvent>

#include "data/Project.hpp"
#include "data/Materials.hpp"
#include "data/Scene.hpp"
#include "controller/MapAction.hpp"
#include "GraphicsShape.hpp"

namespace eno {

GraphicsView::GraphicsView(MapAction* mapAction, QWidget* parent)
	: _mapAction(mapAction)
	, QGraphicsView(parent) {}

void GraphicsView::init() {
	setMouseTracking(true);

	_rect = new QGraphicsRectItem;
	_xAxis = new QGraphicsLineItem;
	_yAxis = new QGraphicsLineItem;

	QPen pen;
	pen.setStyle(Qt::DashLine);
	_xAxis->setPen(pen);
	_yAxis->setPen(pen);

	_shapeNormal = new GraphicsShape(_mapAction);
	_shapeBelow = new GraphicsShape(_mapAction);
	_shapeBelow->setMode(GraphicsShape::Mode::Below);

	// We need to respect the order for the paint event
	_scene = new QGraphicsScene(this);
	_scene->addItem(_shapeBelow);
	_scene->addItem(_shapeNormal);
	_scene->addItem(_rect);
	_scene->addItem(_xAxis);
	_scene->addItem(_yAxis);
	setScene(_scene);

	connect(_mapAction->project()->scene(), &Scene::dataUpdated, this, &GraphicsView::updateShapes);
	connect(_mapAction->project()->scene(), &Scene::rectUpdated, this, &GraphicsView::updateRect);
	connect(_mapAction->project()->materials(), &Materials::materialUpdated, this, &GraphicsView::updateShapes);
	connect(_mapAction, &MapAction::depthUpdated, this, &GraphicsView::updateShapes);
	connect(_mapAction, &MapAction::zoomUpdated, this, &GraphicsView::updateZoom);
}

const QVector2D& GraphicsView::pointerPosition() const {
	return _pointerPosition;
}

void GraphicsView::mousePressEvent(QMouseEvent* e) {
	QGraphicsView::mousePressEvent(e);
	if (e->buttons() == Qt::LeftButton) {
		_mapAction->mousePressEvent(mapToData(e->pos()));
	}
}

void GraphicsView::mouseMoveEvent(QMouseEvent* e) {
	QGraphicsView::mouseMoveEvent(e);
	const auto& pos = mapToData(e->pos());
	_pointerPosition = { pos.x(), pos.z() };
	if (e->buttons() == Qt::LeftButton) {
		_mapAction->mouseMoveEvent(pos);
	}
	pointerPositionUpdated();
}

void GraphicsView::updateShapes() {
	_shapeNormal->update();
	_shapeBelow->update();
}

void GraphicsView::updateRect() {
	const auto* scene = _mapAction->project()->scene();
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
	auto zoom = std::underlying_type_t<MapAction::Zoom>(_mapAction->zoom()) / 100.0;
	transform.scale(zoom, zoom);
	setTransform(transform);
}

const QVector3D GraphicsView::mapToData(const QPoint& pos) const {
	const auto& posF = mapToScene(pos);
	return { floorf(posF.x() / 10.f), _mapAction->depth(), floorf(posF.y() / 10.f) };
}

} // namespace eno
