#include "SceneCommand.hpp"

#include <QtCore/QPoint>

#include <eno/data/Object.hpp>
#include <eno/data/Project.hpp>
#include <eno/data/Scene.hpp>

#include "controller/Commands.hpp"
#include "CreateCommand.hpp"
#include "DestroyCommand.hpp"
#include "ObjectCommand.hpp"
#include "ValueCommand.hpp"

namespace eno {
bool SceneCommand::add(Commands* c, Project* p, const QList<QVector3D>& v, Material* m) {
	// Search:
	// - vec is a new object
	// - vec has already an object but new material
	// - vec has already an object but same material
	auto* s = p->scene();
	QList<QVector3D> oToCreate;
	QList<Object*> oToSwap;
	for (const auto& vec : v) {
		bool found = false;
		for (auto* object : s->objects()) {
			if (object->position() == vec) {
				if (object->material() != m) {
					oToSwap.append(object);
				}
				found = true;
				break;
			}
		}
		if (!found) {
			oToCreate.append(vec);
		}
	}

	// Check if we need to swap or to create an object
	if (oToSwap.isEmpty() && oToCreate.isEmpty()) {
		return false;
	}

	c->beginList();

	// Swap
	for (auto* o : oToSwap) {
		ObjectCommand::setMaterial(c, o, m);
	}

	// Create
	if (!oToCreate.isEmpty()) {
		auto* command = new CreateCommand<Object, Scene, Project>(p->scene(), p, oToCreate.count());
		c->add(command);
		for (int i = 0; i < oToCreate.count(); ++i) {
			auto* o = command->instances()[i];
			o->setPosition(oToCreate.at(i));
			o->setMaterial(m);
		}
	}

	c->endList();

	return true;
}


bool SceneCommand::remove(Commands* c, Scene* s, const QList<QVector3D>& v) {
	QList<Object*> objects;
	for (const auto& vec : v) {
		for (auto* object : s->objects()) {
			if (object->position() == vec) {
				objects.append(object);
				break;
			}
		}
	}
	return remove(c, s, objects);
}

bool SceneCommand::remove(Commands* c, Scene* s, const QList<Object*>& objects) {
	if (objects.isEmpty()) {
		return false;
	}

	c->beginList();

	for (Object* o : objects) {
		ObjectCommand::setMaterial(c, o, nullptr);
	}
	c->add(new DestroyCommand(objects, s));

	c->endList();

	return true;
}

bool SceneCommand::resize(Commands* c, Scene* s, const QPoint& newMin, const QPoint& newMax) {
	const auto& oldMin = s->min();
	const auto& oldMax = s->max();
	const auto& minFixed = s->fixMin(newMin);
	const auto& maxFixed = s->fixMax(newMax);

	// Check validity
	if (oldMin == minFixed && oldMax == maxFixed) {
		return false;
	}

	c->beginList();

	// Resize
	if (oldMin != minFixed)
		c->add(new ValueCommand(s, &Scene::setMin, oldMin, minFixed));
	if (oldMax != maxFixed)
		c->add(new ValueCommand(s, &Scene::setMax, oldMax, maxFixed));

	// Search object to delete
	QList<Object*> o{};
	for (auto* object : s->objects()) {
		const auto& pos = object->position();
		// We use Y-up
		if (pos.x() < minFixed.x() || pos.x() >= maxFixed.x() || pos.z() < minFixed.y() || pos.z() >= maxFixed.y()) {
			o.append(object);
		}
	}
	remove(c, s, o);

	c->endList();

	return true;
}

} // namespace eno