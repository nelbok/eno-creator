#include <eno/data/Project.hpp>

#include <QtCore/QFileInfo>

#include <eno/data/Materials.hpp>
#include <eno/data/Scene.hpp>

namespace eno {
Project::Project(QObject* parent)
	: QObject(parent) {}

void Project::init() {
	assert(!_materials);
	assert(!_scene);
	_materials = new Materials(this);
	_scene = new Scene(this);
}

void Project::reset() {
	assert(_materials);
	assert(_scene);

	_materials->reset();
	_scene->reset();

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

} // namespace eno
