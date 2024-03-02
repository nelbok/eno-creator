#include <eno/data/Material.hpp>

#include <eno/data/Project.hpp>

namespace eno {
Material::Material(Project* project)
	: Item(project) {}

Material::Material(const QUuid& uuid, Project* project)
	: Item(uuid, project) {}

void Material::setName(const QString& name) {
	if (_name != name) {
		_name = name;
		_project->setIsModified(true);
		emit nameUpdated();
	}
}

void Material::setDiffuse(const QColor& color) {
	if (_diffuse != color) {
		_diffuse = color;
		_project->setIsModified(true);
		emit diffuseUpdated();
	}
}
} // namespace eno
