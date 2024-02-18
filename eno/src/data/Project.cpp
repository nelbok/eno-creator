#include <eno/data/Project.hpp>

#include <QtCore/QFileInfo>

#include <eno/data/Material.hpp>
#include <eno/data/Scene.hpp>

namespace eno {
Project::Project(QObject* parent)
	: QObject(parent) {}

Project::~Project() {
	delete _scene;
}

void Project::init() {
	assert(!_scene);
	_scene = new Scene(this);
}

void Project::reset() {
	// Reset Scene
	assert(_scene);
	_scene->reset();

	// Reset materials
	for (auto* material : _materials) {
		material->deleteLater();
	}
	_materials.clear();
	add(new Material(this));

	// Finalize
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
		emit filePathUpdated();
	}
}

void Project::setIsModified(bool value) {
	if (_isModified != value) {
		_isModified = value;
		emit isModifiedUpdated();
	}
}

void Project::add(Material* material) {
	assert(material);
	assert(!_materials.contains(material));
	_materials.append(material);
	setIsModified(true);
	emit materialsUpdated();
}

bool Project::canRemove(Material* material) {
	assert(material);
	assert(_materials.contains(material));
	return material->refCount() == 0 && _materials.count() != 1;
}

void Project::remove(Material* material) {
	assert(material);
	assert(_materials.contains(material));
	if (canRemove(material)) {
		_materials.removeAll(material);
		material->deleteLater();
		setIsModified(true);
		emit materialsUpdated();
	}
}

} // namespace eno
