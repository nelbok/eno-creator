#include "MapAction.hpp"

#include "data/Materials.hpp"
#include "data/Project.hpp"
#include "data/Scene.hpp"

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

MapAction::MapAction(Project* project, QObject* parent)
	: QObject(parent)
	, _project(project) {}


void MapAction::reset() {
	_project->reset();
	setTypeAction(TypeAction::Add);
	setMaterial(*(_project->materials()->begin()));
	setDepth(0.f);
	setPenWidth(1);
	setZoom(Zoom::x100);
}

void MapAction::setTypeAction(TypeAction value) {
	_typeAction = value;
}

MapAction::TypeAction MapAction::typeAction() const {
	return _typeAction;
}

void MapAction::setMaterial(Material* material) {
	assert(material);
	_material = material;
	materialUpdated();
}

Material* MapAction::material() const {
	return _material;
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

const Project* MapAction::project() const {
	assert(_project);
	return _project;
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

const Qt::CursorShape MapAction::cursorShape(const QVector3D& pos) const {
	auto shape = Qt::CursorShape::ArrowCursor;
	switch (_typeAction) {
		case TypeAction::Remove:
			// Nothing to do
			break;
		case TypeAction::Add:
			// Nothing to do
			break;
		case TypeAction::Picker:
			shape = Qt::CrossCursor;
			break;
		case TypeAction::Resize:
			shape = Qt::SizeAllCursor;
			break;
	}
	return shape;
}

bool MapAction::validPosition(const QVector3D& pos) const {
	auto* scene = _project->scene();
	return (scene->min().x() <= pos.x() && pos.x() < scene->max().x()) && (scene->min().y() <= pos.z() && pos.z() < scene->max().y());
}

void MapAction::removeItem(const QVector3D& pos) {
	changeItem(pos, [this](const QVector3D& vec) {
		this->_project->scene()->removeItem(vec);
	});
}

void MapAction::addItem(const QVector3D& pos) {
	changeItem(pos, [this](const QVector3D& vec) {
		this->_project->scene()->addItem(vec, this->_material);
	});
}

void MapAction::pickColor(const QVector3D& pos) {
	if (_project->scene()->findItem(pos)) {
		_material = _project->scene()->materialAt(pos);
		materialUpdated();
	}
}

void MapAction::resize(const QVector2D& pos) {
	if (_currentPos == pos) {
		return;
	}

	bool changed = false;
	QPoint min = _project->scene()->min();
	QPoint max = _project->scene()->max();

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
		_project->scene()->setMin(min);
		_project->scene()->setMax(max);
	}

	_currentPos = pos;
}

} // namespace eno
