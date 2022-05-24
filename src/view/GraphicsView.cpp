#include "GraphicsView.hpp"

#include <cmath>

#include <QMouseEvent>

#include "data/Data.hpp"
#include "controller/MapAction.hpp"
#include "GraphicsItem.hpp"
#include "Utils.hpp"

namespace eno {

GraphicsView::GraphicsView(MapAction* mapAction, QWidget* parent)
	: _mapAction(mapAction)
	, QGraphicsView(parent) {}

void GraphicsView::init() {
	QPen pen;
	pen.setStyle(Qt::DashLine);
	_xAxis.setPen(pen);
	_yAxis.setPen(pen);

	_scene = new QGraphicsScene(this);
	_scene->addItem(&_rect);
	_scene->addItem(&_xAxis);
	_scene->addItem(&_yAxis);

	setScene(_scene);

	connect(_mapAction->data(), &Data::sceneUpdated, this, &GraphicsView::updateMap);
	connect(_mapAction->data(), &Data::rectUpdated, this, &GraphicsView::updateRect);
	connect(_mapAction, &MapAction::depthUpdated, this, &GraphicsView::updateMap);
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

void GraphicsView::updateMap() {
	for (auto it = _items.begin(); it != _items.end(); ++it) {
		if (!_mapAction->data()->findItem(it.key())) {
			it.value()->hide();
		}
	}

	for (const auto& pair : *(_mapAction->data())) {
		if (pair.first.z() == _mapAction->depth() || pair.first.z() == _mapAction->depth() - 1) {
			if (_items.contains(pair.first)) {
				auto* item = _items.value(pair.first);
				item->show();
				item->setColor(pair.second);
				item->update();
			} else {
				auto* item = new GraphicsItem(_mapAction);
				item->setPos(pair.first);
				item->setColor(pair.second);
				_scene->addItem(item);
				_items.insert(pair.first, item);
			}
		}
	}
}

void GraphicsView::updateRect() {
	const auto* data = _mapAction->data();
	const auto& min = data->min() * 10.f;
	const auto& max = data->max() * 10.f;

	const QRectF rect{ min.x(), min.y(), max.x() - min.x(), max.y() - min.y() };
	_scene->setSceneRect(rect);
	_rect.setRect(rect);
	_xAxis.setLine(min.x() - 30.f, 0.f, max.x() + 30.f, 0.f);
	_yAxis.setLine(0, min.y() - 30.f, 0, max.y() + 30.f);
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
