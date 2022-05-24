#include "MapAction.hpp"

#include <QVector2D>

#include "data/Data.hpp"

namespace eno {

QString MapAction::toString(MapAction::Zoom zoom) {
	switch (zoom) {
		case MapAction::Zoom::x10:
			return "x10";
		case MapAction::Zoom::x25:
			return "x25";
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

void MapAction::mousePressEvent(const QVector2D& pos) {
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
			_currentPos = pos;
			break;
	}
}

void MapAction::mouseMoveEvent(const QVector2D& pos) {
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
			resize(pos);
			break;
	}
}

bool MapAction::validPosition(const QVector3D& pos) const {
	return (_data->min().x() <= pos.x() && pos.x() < _data->max().x()) && (_data->min().y() <= pos.y() && pos.y() < _data->max().y());
}

void MapAction::removeItem(const QVector2D& pos) {
	changeItem(pos, [this](const QVector3D& vec) {
		this->_data->removeItem(vec);
	});
}

void MapAction::addItem(const QVector2D& pos) {
	changeItem(pos, [this](const QVector3D& vec) {
		this->_data->addItem(vec, this->_color);
	});
}

void MapAction::pickColor(const QVector2D& pos) {
	const QVector3D vec{ pos, _depth };
	if (_data->findItem(vec)) {
		_color = _data->colorAt(vec);
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

//    int line = 0;
//    if (pos_x - 5 < creator_->map().x_min() && pos_x + 5 > creator_->map().x_min())
//      line |= 0x1000;
//    if (pos_x - 5 < creator_->map().x_max() && pos_x + 5 > creator_->map().x_max())
//      line |= 0x0100;
//    if (pos_y - 5 < creator_->map().y_min() && pos_y + 5 > creator_->map().y_min())
//      line |= 0x0010;
//    if (pos_y - 5 < creator_->map().y_max() && pos_y + 5 > creator_->map().y_max())
//      line |= 0x0001;

//    QCursor cursor = creator_->cursor();
//    cursor.setShape(Qt::ArrowCursor);
//    if ((line & 0x1000) == 0x1000 || (line & 0x0100) == 0x0100)
//      cursor.setShape(Qt::SizeHorCursor);
//    if ((line & 0x0010) == 0x0010 || (line & 0x0001) == 0x0001)
//      cursor.setShape(Qt::SizeVerCursor);
//    if ((line & 0x1010) == 0x1010 || (line & 0x0101) == 0x0101)
//      cursor.setShape(Qt::SizeFDiagCursor);
//    if ((line & 0x0110) == 0x0110 || (line & 0x1001) == 0x1001)
//      cursor.setShape(Qt::SizeBDiagCursor);
//    creator_->setCursor(cursor);
//  }

} // namespace eno
