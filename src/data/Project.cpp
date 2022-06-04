#include "Project.hpp"

#include <QFileInfo>

#include "Material.hpp"
#include "Scene.hpp"

namespace eno {
Project::Project(QObject* parent)
	: QObject(parent) {}

void Project::init() {
	assert(!_scene);
	_scene = new Scene(this);
}

void Project::reset() {
	assert(_scene);
	_scene->reset();

	Material* defaultMaterial = new Material(this);
	defaultMaterial->setName("default");
	defaultMaterial->setDiffuse(QColor("#ffaa00"));
	_materials.append(defaultMaterial);

	setFilePath("");

	setIsModified(false);
}

QString Project::projectName() const {
	if (!_filePath.isEmpty()) {
		const auto& fileInfo = QFileInfo(_filePath);
		if (fileInfo.exists()) {
			return fileInfo.baseName();
		}
	}
	return "New Project";
}

void Project::setFilePath(const QString& path) {
	if (_filePath != path) {
		_filePath = path;
		filePathUpdated();
	}
}

void Project::setIsModified(bool value) {
	if (_isModified != value) {
		_isModified = value;
		isModifiedUpdated();
	}
}

const QList<Material*>& Project::materials() const {
	assert(!_materials.empty());
	return _materials;
}

void Project::addMaterial(Material* material) {
	_materials.append(material);
}

void Project::removeMaterial(Material* material) {
	_materials.removeAll(material);
}

} // namespace eno
