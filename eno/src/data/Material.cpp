#include <eno/data/Material.hpp>

#include <eno/data/Project.hpp>
#include <eno/data/Preferences.hpp>

namespace eno {
Material::Material(Project* project)
	: QObject(project)
	, _project{ project }
	, _name{ Preferences::materialName() }
	, _diffuse{ Preferences::materialDiffuse() } {}

Material::~Material() {
	assert(_refCount == 0);
}

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

void Material::increaseRefCount() {
	++_refCount;
	emit refCountUpdated();
}

void Material::decreaseRefCount() {
	assert(_refCount > 0);
	--_refCount;
	emit refCountUpdated();
}
} // namespace eno
