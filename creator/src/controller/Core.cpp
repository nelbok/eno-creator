#include "Core.hpp"

#include <eno/data/Material.hpp>
#include <eno/data/Project.hpp>

#include "command/MaterialCommand.hpp"
#include "command/TextureCommand.hpp"
#include "Commands.hpp"
#include "Graphics.hpp"
#include "Preferences.hpp"
#include "RecentFiles.hpp"

namespace eno {

Core::Core(Project* project, QObject* parent)
	: QObject(parent)
	, _commands(new Commands(this))
	, _graphics(new Graphics(this))
	, _project(project)
	, _recentFiles(new RecentFiles(this)) {}

void Core::reset() {
	// Clean commands and project
	_commands->reset();
	_project->reset();
	_project->setName("New project");

	// Scene
	_project->scene()->setMin(Preferences::sceneMin());
	_project->scene()->setMax(Preferences::sceneMax());
	emit _project->scene()->rectUpdated();

	// Create material
	createMyMaterial();
	_project->setIsModified(false);

	// Other controllers
	_graphics->reset();
	_commands->reset();
	_recentFiles->reset();
}

Material* Core::createMyMaterial() {
	auto* material = MaterialCommand::create(_commands, _project);
	material->setName(Preferences::materialName());
	material->setDiffuse(Preferences::materialDiffuse());
	return material;
}

Texture* Core::createMyTexture() {
	auto* texture = TextureCommand::create(_commands, _project);
	texture->setName("My Texture");
	texture->setData(QPixmap(":/logo/logo.png"));
	return texture;
}

Graphics* Core::graphics() const {
	assert(_graphics);
	return _graphics;
}

Project* Core::project() const {
	assert(_project);
	return _project;
}

Commands* Core::commands() const {
	assert(_commands);
	return _commands;
}

RecentFiles* Core::recentFiles() const {
	assert(_recentFiles);
	return _recentFiles;
}

} // namespace eno
