#include "Graphics.hpp"

#include <eno/data/Material.hpp>
#include <eno/data/Object.hpp>
#include <eno/data/Project.hpp>
#include <eno/data/Scene.hpp>

#include "controller/command/SceneCommand.hpp"
#include "controller/Commands.hpp"
#include "controller/Core.hpp"

namespace eno {

Graphics::Graphics(Core* core, QObject* parent)
	: QObject(parent)
	, _core(core){}

void Graphics::reset() {
	setTypeAction(Preferences::TypeAction::Add);
	setMaterial(*(_core->project()->materials().begin()));
	setDepth(Preferences::mapDepth());
	setPenWidth(Preferences::mapPenWidth());
	setZoom(Preferences::mapZoom());
}

void Graphics::setTypeAction(Preferences::TypeAction value) {
	_typeAction = value;
}

Preferences::TypeAction Graphics::typeAction() const {
	return _typeAction;
}

void Graphics::setMaterial(Material* material) {
	assert(material);
	_material = material;
	emit materialUpdated();
}

Material* Graphics::material() const {
	return _material;
}

void Graphics::setDepth(int depth) {
	_depth = depth;
	emit depthUpdated();
}

int Graphics::depth() const {
	return _depth;
}

void Graphics::setPenWidth(int penWidth) {
	_penWidth = penWidth;
	emit penWidthUpdated();
}

int Graphics::penWidth() const {
	return _penWidth;
}

Preferences::Zoom Graphics::zoom() const {
	return _zoom;
}

void Graphics::setZoom(Preferences::Zoom zoom) {
	_zoom = zoom;
	emit zoomUpdated();
}

void Graphics::mousePressEvent(const QVector3D& pos) {
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

void Graphics::mouseMoveEvent(const QVector3D& pos) {
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

Qt::CursorShape Graphics::cursorShape() const {
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

bool Graphics::validPosition(const QVector3D& pos) const {
	auto* scene = _core->project()->scene();
	return (scene->min().x() <= pos.x() && pos.x() < scene->max().x()) && (scene->min().y() <= pos.z() && pos.z() < scene->max().y());
}

void Graphics::removeItem(const QVector3D& pos) {
	changeItem(pos, [this](const QList<QVector3D>& vecs) {
		SceneCommand::remove(_core->commands(), _core->project()->scene(), vecs);
	});
}

void Graphics::addItem(const QVector3D& pos) {
	changeItem(pos, [this](const QList<QVector3D>& vecs) {
		SceneCommand::add(_core->commands(), _core->project(), vecs, _material);
	});
}

void Graphics::pickColor(const QVector3D& pos) {
	const auto& objects = _core->project()->scene()->objects();
	for (auto* object : objects) {
		if (object->position() == pos) {
			_material = object->material();
			emit materialUpdated();
		}
	}
}

void Graphics::resize(const QVector2D& pos) {
	if (_currentPos == pos) {
		return;
	}

	bool changed = false;
	auto* scene = _core->project()->scene();
	QPoint min = scene->min();
	QPoint max = scene->max();

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
		SceneCommand::resize(_core->commands(), scene, min, max);
	}

	_currentPos = pos;
}

} // namespace eno
