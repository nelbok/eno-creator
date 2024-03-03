#include <eno/data/Texture.hpp>

#include <eno/data/Project.hpp>

namespace eno {
Texture::Texture(Project* project)
	: Item(project) {}

Texture::Texture(const QUuid& uuid, Project* project)
	: Item(uuid, project) {}

void Texture::setName(const QString& name) {
	if (_name != name) {
		_name = name;
		_project->setIsModified(true);
		emit nameUpdated();
	}
}

void Texture::setData(const QPixmap& data) {
	// Restrict data
	// Size
	_data = data;
	_project->setIsModified(true);
	emit dataUpdated();
}
} // namespace eno
