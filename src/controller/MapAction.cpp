#include "MapAction.hpp"

#include <QVector2D>

#include "data/Data.hpp"

namespace eno {

QString MapAction::toString(MapAction::Zoom zoom) {
	switch (zoom) {
		case MapAction::Zoom::x50:
			return " x50";
		case MapAction::Zoom::x100:
			return " x100";
		case MapAction::Zoom::x200:
			return " x200";
		case MapAction::Zoom::x400:
			return " x400";
	}
	assert(false);
	return {};
}

MapAction::MapAction(Data* data, QObject* parent)
	: QObject(parent)
	, _data(data) {}


void MapAction::reset() {
	setTypeAction(TypeAction::Add);
	setColor(QColor("#ffaa00"));
	setDepth(0.f);
	setPenWidth(1);
	setZoom(Zoom::x100);
	_data->reset();
}

void MapAction::setTypeAction(TypeAction value) {
	_typeAction = value;
}

MapAction::TypeAction MapAction::typeAction() const {
	return _typeAction;
}

void MapAction::setColor(const QColor& color) {
	_color = color;
	colorUpdated();
}
const QColor& MapAction::color() const {
	return _color;
}

void MapAction::setDepth(float depth) {
	_depth = depth;
	depthUpdated();
}

float MapAction::depth() const {
	return _depth;
}

void MapAction::setPenWidth(int penWidth) {
	_penWidth = penWidth;
	penWidthUpdated();
}

int MapAction::penWidth() const {
	return _penWidth;
}

MapAction::Zoom MapAction::zoom() const {
	return _zoom;
}

void MapAction::setZoom(MapAction::Zoom zoom) {
	_zoom = zoom;
	zoomUpdated();
}

const Data* MapAction::data() const {
	assert(_data);
	return _data;
}

void MapAction::mousePressEvent(const QVector3D& pos) {
	switch (_typeAction) {
		case TypeAction::Remove:
			removeItem(pos);
			break;
		case TypeAction::Add:
			addItem(pos);
			break;
		case TypeAction::Picker:
			pickColor(pos);
			break;
		case TypeAction::Resize:
			_currentPos = { pos.x(), pos.z() };
			break;
	}
}

void MapAction::mouseMoveEvent(const QVector3D& pos) {
	switch (_typeAction) {
		case TypeAction::Remove:
			removeItem(pos);
			break;
		case TypeAction::Add:
			addItem(pos);
			break;
		case TypeAction::Picker:
			// Nothing to do
			break;
		case TypeAction::Resize:
			resize({ pos.x(), pos.z() });
			break;
	}
}

bool MapAction::validPosition(const QVector3D& pos) const {
	return (_data->min().x() <= pos.x() && pos.x() < _data->max().x()) && (_data->min().y() <= pos.z() && pos.z() < _data->max().y());
}

void MapAction::removeItem(const QVector3D& pos) {
	changeItem(pos, [this](const QVector3D& vec) {
		this->_data->removeItem(vec);
	});
}

void MapAction::addItem(const QVector3D& pos) {
	changeItem(pos, [this](const QVector3D& vec) {
		this->_data->addItem(vec, this->_color);
	});
}

void MapAction::pickColor(const QVector3D& pos) {
	if (_data->findItem(pos)) {
		_color = _data->colorAt(pos);
		colorUpdated();
	}
}

void MapAction::resize(const QVector2D& pos) {
	if (_currentPos == pos) {
		return;
	}

	bool changed = false;
	auto min = _data->min();
	auto max = _data->max();

	if (_currentPos.x() == min.x() && std::abs(_currentPos.x() - pos.x()) <= 1.f) {
		min.setX(pos.x());
		changed = true;
	}
	if (_currentPos.x() == max.x() && std::abs(_currentPos.x() - pos.x()) <= 1.f) {
		max.setX(pos.x());
		changed = true;
	}
	if (_currentPos.y() == min.y() && std::abs(_currentPos.y() - pos.y()) <= 1.f) {
		min.setY(pos.y());
		changed = true;
	}
	if (_currentPos.y() == max.y() && std::abs(_currentPos.y() - pos.y()) <= 1.f) {
		max.setY(pos.y());
		changed = true;
	}

	if (changed) {
		_data->setMin(min);
		_data->setMax(max);
	}

	_currentPos = pos;
}

} // namespace eno
