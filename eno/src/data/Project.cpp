#include <eno/data/Project.hpp>

#include <eno/data/Scene.hpp>

namespace eno {
Project::Project(QObject* parent)
	: QObject(parent)
	, Container<Material, Project>(this)
	, Container<Texture, Project>(this) {}

Project::~Project() {
	clear();
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

	// Reset tags
	_tags.clear();
	emit tagsUpdated(_tags);

	// Materials and textures
	clear();

	// Finalize
	setName("");
	setFilePath("");
	setIsModified(false);
}

void Project::setName(const QString& name) {
	if (_name != name) {
		_name = name;
		setIsModified(true);
		emit nameUpdated();
	}
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

void Project::add(const QStringList& tags) {
	for (const auto& tag : tags) {
		_tags.append(tag);
	}
	setIsModified(true);
	emit tagsUpdated(_tags);
}

void Project::remove(const QStringList& tags) {
	for (const auto& tag : tags) {
		assert(_tags.contains(tag));
		_tags.removeOne(tag);
	}
	emit tagsUpdated(_tags);
}

void Project::datasUpdated(const QList<Material*>& datas) {
	emit materialsUpdated(datas);
}

void Project::datasUpdated(const QList<Texture*>& datas) {
	emit texturesUpdated(datas);
}

void Project::clear() {
	// Materials
	for (auto* data : Container<Material, Project>::_datas) {
		data->setTexture(nullptr);
	}
	Container<Material, Project>::clear();

	// Textures
	Container<Texture, Project>::clear();
}

} // namespace eno
