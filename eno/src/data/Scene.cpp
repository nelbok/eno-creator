#include <eno/data/Scene.hpp>

#include <eno/data/Object.hpp>
#include <eno/data/Project.hpp>
#include <eno/tools/Utils.hpp>

namespace eno {
Scene::Scene(Project* project)
	: QObject(project)
	, _project(project) {}

Scene::~Scene() {
	clear();
}

void Scene::reset() {
	clear();
}

void Scene::clear() {
	for (auto* object : _objects) {
		// We can't use deleteLater
		// Material needs to be informed immediately when an object is about to be deleted
		delete object;
	}
	_objects.clear();
}

void Scene::setMin(const QPoint& min) {
	if (_min != min) {
		auto v = Scene::defaultSceneMin;
		_min = min;
		if (_min.x() > v.x()) {
			_min.setX(v.x());
		}
		if (_min.y() > v.y()) {
			_min.setY(v.y());
		}
		_project->setIsModified(true);
		updateScene();
		emit rectUpdated();
	}
}

void Scene::setMax(const QPoint& max) {
	if (_max != max) {
		auto v = Scene::defaultSceneMax;
		_max = max;
		if (_max.x() < v.x()) {
			_max.setX(v.x());
		}
		if (_max.y() < v.y()) {
			_max.setY(v.y());
		}
		_project->setIsModified(true);
		updateScene();
		emit rectUpdated();
	}
}

void Scene::add(const QList<Object*>& objects) {
	QList<Object*> objectsToDelete{};
	for (auto* o1 : objects) {
		assert(o1);

		// Check if another object is at that position
		for (auto* o2 : _objects) {
			if (o2->position() == o1->position()) {
				assert(o2 != o1);
				objectsToDelete.append(o2);
				break;
			}
		}
	}

	blockSignals(true);
	remove(objectsToDelete);
	blockSignals(false);

	// Add the object
	_objects.append(objects);
	_project->setIsModified(true);
	emit objectsUpdated();
}

void Scene::remove(const QList<Object*>& objects) {
	for (auto* object : objects) {
		assert(object);
		assert(_objects.contains(object));
		_objects.removeAll(object);
		// We can't use deleteLater
		// Material needs to be informed immediately when an object is about to be deleted
		delete object;
	}
	_project->setIsModified(true);
	emit objectsUpdated();
}

void Scene::updateScene() {
	QList<Object*> objectsToDelete{};
	for (auto* object : _objects) {
		const auto& pos = object->position();
		if (pos.x() < _min.x() || pos.x() > _max.x() || pos.y() < _min.y() || pos.y() > _max.y()) {
			objectsToDelete.append(object);
		}
	}

	blockSignals(true);
	remove(objectsToDelete);
	blockSignals(false);

	if (!objectsToDelete.isEmpty()) {
		_project->setIsModified(true);
		emit objectsUpdated();
	}
}
} // namespace eno
