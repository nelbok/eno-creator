#include <eno/data/Object.hpp>

#include <eno/data/Material.hpp>
#include <eno/data/Project.hpp>
#include <eno/data/Scene.hpp>

namespace eno {
Object::Object(Project* project)
	: QObject(project)
	, _project(project) {}

Object::~Object() {
	setMaterial(nullptr);
}

void Object::setPosition(const QVector3D& position) {
	if (_position != position) {
		_position = position;
		_project->setIsModified(true);
		emit positionUpdated();
	}
}

void Object::setMaterial(Material* material) {
	if (_material != material) {
		// Refcount
		if (_material)
			_material->decreaseRefCount();
		if (material)
			material->increaseRefCount();

		_material = material;
		_project->setIsModified(true);
		emit materialUpdated();
	}
}


} // namespace eno
