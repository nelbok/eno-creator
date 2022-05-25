#include "GraphicsView.hpp"

#include <cmath>

#include <QGraphicsRectItem>
#include <QGraphicsLineItem>
#include <QMouseEvent>

#include "data/Data.hpp"
#include "controller/MapAction.hpp"
#include "GraphicsShape.hpp"

namespace eno {

GraphicsView::GraphicsView(MapAction* mapAction, QWidget* parent)
	: _mapAction(mapAction)
	, QGraphicsView(parent) {}

void GraphicsView::init() {
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

	connect(_mapAction->data(), &Data::sceneUpdated, this, &GraphicsView::updateShapes);
	connect(_mapAction->data(), &Data::rectUpdated, this, &GraphicsView::updateRect);
	connect(_mapAction, &MapAction::depthUpdated, this, &GraphicsView::updateShapes);
	connect(_mapAction, &MapAction::zoomUpdated, this, &GraphicsView::updateZoom);
}

void GraphicsView::mousePressEvent(QMouseEvent* e) {
	QGraphicsView::mousePressEvent(e);
	if (e->buttons() == Qt::LeftButton) {
		_mapAction->mousePressEvent(mapToData(e->pos()));
	}
}

void GraphicsView::mouseMoveEvent(QMouseEvent* e) {
	QGraphicsView::mouseMoveEvent(e);
	if (e->buttons() == Qt::LeftButton) {
		_mapAction->mouseMoveEvent(mapToData(e->pos()));
	}
}

void GraphicsView::updateShapes() {
	_shapeNormal->update();
	_shapeBelow->update();
}

void GraphicsView::updateRect() {
	const auto* data = _mapAction->data();
	const auto& min = data->min() * 10.f;
	const auto& max = data->max() * 10.f;

	const QRectF rect{ min.x(), min.y(), max.x() - min.x(), max.y() - min.y() };
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

const QVector2D GraphicsView::mapToData(const QPoint& pos) const {
	const auto& posF = mapToScene(pos);
	return { std::floorf(posF.x() / 10.f), std::floorf(posF.y() / 10.f) };
}

} // namespace eno
