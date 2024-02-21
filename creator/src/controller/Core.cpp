#include "Core.hpp"

#include <eno/data/Material.hpp>
#include <eno/data/Project.hpp>

#include "command/MaterialCommand.hpp"
#include "Graphics.hpp"
#include "Preferences.hpp"
#include "UndoRedo.hpp"

namespace eno {

Core::Core(Project* project, QObject* parent)
	: QObject(parent)
	, _graphics(new Graphics(project))
	, _project(project)
	, _undoRedo(new UndoRedo(this)) {}


void Core::reset() {
	// First the project
	_project->reset();

	// Scene
	_project->scene()->setMin(Preferences::sceneMin());
	_project->scene()->setMax(Preferences::sceneMax());

	// Create material
	createMyMaterial();
	_project->setIsModified(false);

	// Other controllers
	_graphics->reset();
	_undoRedo->reset();
}

Material* Core::createMyMaterial() {
	auto* material = MaterialCommand::create(_undoRedo, _project);
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

UndoRedo* Core::undoRedo() const {
	assert(_undoRedo);
	return _undoRedo;
}

} // namespace eno
