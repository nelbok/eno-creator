#include <eno/data/Project.hpp>

#include <QtCore/QFileInfo>

#include <eno/data/Scene.hpp>

namespace eno {
Project::Project(QObject* parent)
	: QObject(parent)
	, Container(this) {}

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
	Container::clear();

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

void Project::setFilePath(const QString& filePath) {
	if (_filePath != filePath) {
		_filePath = filePath;
		emit filePathUpdated();
	}
}

void Project::setIsModified(bool value) {
	if (_isModified != value) {
		_isModified = value;
		emit isModifiedUpdated();
	}
}

void Project::datasUpdated() {
	emit materialsUpdated();
}

} // namespace eno
