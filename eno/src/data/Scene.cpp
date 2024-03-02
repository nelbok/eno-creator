#include <eno/data/Scene.hpp>

#include <eno/data/Project.hpp>

namespace eno {
Scene::Scene(Project* project)
	: QObject(project)
	, Container<Object, Project>(project) {}

Scene::~Scene() {
	clear();
}

void Scene::reset() {
	clear();
	_min = Scene::defaultSceneMin;
	_max = Scene::defaultSceneMax;
}

void Scene::clear() {
	for (auto* object : _datas) {
		object->setMaterial(nullptr);
	}
	Container<Object, Project>::clear();
}

void Scene::setMin(const QPoint& min) {
	const auto& value = fixMin(min);
	if (_min != value) {
		_min = value;
		_project->setIsModified(true);
		emit rectUpdated();
	}
}

void Scene::setMax(const QPoint& max) {
	const auto& value = fixMax(max);
	if (_max != value) {
		_max = value;
		_project->setIsModified(true);
		emit rectUpdated();
	}
}

QPoint Scene::fixMin(QPoint min) const {
	auto v = Scene::defaultSceneMin;
	if (min.x() > v.x()) {
		min.setX(v.x());
	}
	if (min.y() > v.y()) {
		min.setY(v.y());
	}
	return min;
}

QPoint Scene::fixMax(QPoint max) const {
	auto v = Scene::defaultSceneMax;
	if (max.x() < v.x()) {
		max.setX(v.x());
	}
	if (max.y() < v.y()) {
		max.setY(v.y());
	}
	return max;
}

void Scene::datasUpdated(const QList<Object*>& datas) {
	emit objectsUpdated(datas);
}
} // namespace eno
