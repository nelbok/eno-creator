#include "Scene.hpp"

#include "data/Material.hpp"
#include "data/Project.hpp"
#include "Utils.hpp"

namespace eno {
Scene::Scene(Project* project)
	: QObject(project)
	, _project(project) {}

void Scene::reset() {
	setMin({ -5, -5 });
	setMax({ 5, 5 });

	_sceneData.clear();
	dataUpdated();
}

void Scene::setMin(const QPoint& min) {
	if (_min != min) {
		_min = min;
		if (_min.x() > -5) {
			_min.setX(-5);
		}
		if (_min.y() > -5) {
			_min.setY(-5);
		}
		_project->setIsModified(true);
		updateScene();
		rectUpdated();
	}
}
void Scene::setMax(const QPoint& max) {
	if (_max != max) {
		_max = max;
		if (_max.x() < 5) {
			_max.setX(5);
		}
		if (_max.y() < 5) {
			_max.setY(5);
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
