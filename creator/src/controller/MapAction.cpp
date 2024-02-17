#include "MapAction.hpp"

#include <eno/data/Materials.hpp>
#include <eno/data/Project.hpp>
#include <eno/data/Scene.hpp>

namespace eno {

MapAction::MapAction(Project* project, QObject* parent)
	: QObject(parent)
	, _project(project) {}


void MapAction::reset() {
	_project->reset();
	setTypeAction(Preferences::TypeAction::Add);
	setMaterial(*(_project->materials()->begin()));
	setDepth(Preferences::mapActionDepth());
	setPenWidth(Preferences::mapActionPenWidth());
	setZoom(Preferences::mapActionZoom());
}

void MapAction::setTypeAction(Preferences::TypeAction value) {
	_typeAction = value;
}

Preferences::TypeAction MapAction::typeAction() const {
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

void MapAction::setDepth(int depth) {
	_depth = depth;
	depthUpdated();
}

int MapAction::depth() const {
	return _depth;
}

void MapAction::setPenWidth(int penWidth) {
	_penWidth = penWidth;
	penWidthUpdated();
}

int MapAction::penWidth() const {
	return _penWidth;
}

Preferences::Zoom MapAction::zoom() const {
	return _zoom;
}

void MapAction::setZoom(Preferences::Zoom zoom) {
	_zoom = zoom;
	zoomUpdated();
}

const Project* MapAction::project() const {
	assert(_project);
	return _project;
}

Project* MapAction::project() {
	assert(_project);
	return _project;
}

void MapAction::mousePressEvent(const QVector3D& pos) {
	switch (_typeAction) {
		case Preferences::TypeAction::Remove:
			removeItem(pos);
			break;
		case Preferences::TypeAction::Add:
			addItem(pos);
			break;
		case Preferences::TypeAction::Picker:
			pickColor(pos);
			break;
		case Preferences::TypeAction::Resize:
			_currentPos = { pos.x(), pos.z() };
			break;
	}
}

void MapAction::mouseMoveEvent(const QVector3D& pos) {
	switch (_typeAction) {
		case Preferences::TypeAction::Remove:
			removeItem(pos);
			break;
		case Preferences::TypeAction::Add:
			addItem(pos);
			break;
		case Preferences::TypeAction::Picker:
			// Nothing to do
			break;
		case Preferences::TypeAction::Resize:
			resize({ pos.x(), pos.z() });
			break;
	}
}

Qt::CursorShape MapAction::cursorShape() const {
	auto shape = Qt::CursorShape::ArrowCursor;
	switch (_typeAction) {
		case Preferences::TypeAction::Remove:
			// Nothing to do
			break;
		case Preferences::TypeAction::Add:
			// Nothing to do
			break;
		case Preferences::TypeAction::Picker:
			shape = Qt::CrossCursor;
			break;
		case Preferences::TypeAction::Resize:
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
