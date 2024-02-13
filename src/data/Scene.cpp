#include "Scene.hpp"

#include "data/Material.hpp"
#include "data/Project.hpp"
#include "data/Preferences.hpp"
#include "Utils.hpp"

namespace eno {
Scene::Scene(Project* project)
	: QObject(project)
	, _project(project) {}

void Scene::reset() {
	setMin(Preferences::sceneMin());
	setMax(Preferences::sceneMax());

	_sceneData.clear();
	dataUpdated();
}

void Scene::setMin(const QPoint& min) {
	if (_min != min) {
		auto v = Preferences::defaultSceneMin;
		_min = min;
		if (_min.x() > v.x()) {
			_min.setX(v.x());
		}
		if (_min.y() > v.y()) {
			_min.setY(v.y());
		}
		_project->setIsModified(true);
		updateScene();
		rectUpdated();
	}
}
void Scene::setMax(const QPoint& max) {
	if (_max != max) {
		auto v = Preferences::defaultSceneMax;
		_max = max;
		if (_max.x() < v.x()) {
			_max.setX(v.x());
		}
		if (_max.y() < v.y()) {
			_max.setY(v.y());
		}
		_project->setIsModified(true);
		updateScene();
		rectUpdated();
	}
}

void Scene::addItem(const QVector3D& pos, Material* material) {
	assert(material);
	if (findItem(pos)) {
		if (_sceneData[pos] == material) {
			return;
		}
		materialAt(pos)->decreaseRefCount();
	}
	material->increaseRefCount();
	_sceneData[pos] = material;
	_project->setIsModified(true);
	dataUpdated();
}

void Scene::removeItem(const QVector3D& pos) {
	if (_sceneData.contains(pos)) {
		materialAt(pos)->decreaseRefCount();
		_sceneData.remove(pos);
		_project->setIsModified(true);
		dataUpdated();
	}
}

bool Scene::findItem(const QVector3D& pos) const {
	return _sceneData.contains(pos);
}

int Scene::countItems() const {
	return _sceneData.count();
}

Material* Scene::materialAt(const QVector3D& pos) const {
	assert(findItem(pos));
	return _sceneData.value(pos, {});
}

SceneData::const_key_value_iterator Scene::begin() const {
	return _sceneData.constKeyValueBegin();
}

SceneData::const_key_value_iterator Scene::end() const {
	return _sceneData.constKeyValueEnd();
}

void Scene::updateScene() {
	QList<QVector3D> itemsToDelete{};
	for (const auto& item : _sceneData.keys()) {
		if (item.x() < _min.x() || item.x() > _max.x() || item.y() < _min.y() || item.y() > _max.y()) {
			itemsToDelete.append(item);
		}
	}
	for (const auto& item : itemsToDelete) {
		removeItem(item);
	}
	if (!itemsToDelete.isEmpty()) {
		_project->setIsModified(true);
		dataUpdated();
	}
}
} // namespace eno
