#include "Core.hpp"

#include <eno/data/Material.hpp>
#include <eno/data/Project.hpp>

#include "command/MaterialCommand.hpp"
#include "Commands.hpp"
#include "Graphics.hpp"
#include "Preferences.hpp"

namespace eno {

Core::Core(Project* project, QObject* parent)
	: QObject(parent)
	, _commands(new Commands(this))
	, _graphics(new Graphics(this))
	, _project(project) {}

void Core::reset() {
	// Clean commands and project
	_commands->reset();
	_project->reset();

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
}

Material* Core::createMyMaterial() {
	auto* material = MaterialCommand::create(_commands, _project);
	material->setName(Preferences::materialName());
	material->setDiffuse(Preferences::materialDiffuse());
	return material;
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

} // namespace eno
