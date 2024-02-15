#include <eno/data/Materials.hpp>

#include <eno/data/Material.hpp>
#include <eno/data/Project.hpp>

namespace eno {
Materials::Materials(Project* project)
	: QObject(project)
	, _project(project) {}

void Materials::reset() {
	for (auto* material : _materials) {
		material->deleteLater();
	}
	_materials.clear();
	add(new Material(_project));
}

Material* Materials::createMaterial() const {
	return new Material(_project);
}

void Materials::add(Material* material) {
	assert(material);
	assert(!_materials.contains(material));
	_materials.append(material);
	_project->setIsModified(true);
	updated();
}

bool Materials::canRemove(Material* material) {
	assert(material);
	assert(_materials.contains(material));
	return material->refCount() == 0 && _materials.count() != 1;
}
void Materials::remove(Material* material) {
	assert(material);
	assert(_materials.contains(material));
	if (canRemove(material)) {
		_materials.removeAll(material);
		material->deleteLater();
		_project->setIsModified(true);
		updated();
	}
}

int Materials::count() const {
	return _materials.count();
}

QList<Material*>::const_iterator Materials::begin() const {
	return _materials.begin();
}

QList<Material*>::const_iterator Materials::end() const {
	return _materials.end();
}
} // namespace eno
