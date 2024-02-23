#include "ObjectCommand.hpp"

#include <eno/data/Object.hpp>
#include <eno/data/Project.hpp>
#include <eno/data/Scene.hpp>

#include "controller/Commands.hpp"
#include "CreateCommand.hpp"
#include "DestroyCommand.hpp"
#include "ValueCommand.hpp"

namespace eno {
Object* ObjectCommand::create(Commands* c, Project* p) {
	auto* command = new CreateCommand<Object, Scene, Project>(p->scene(), p, 1);
	c->add(command);
	return command->instances().first();
}

bool ObjectCommand::destroy(Commands* c, Object* m, Scene* s) {
	if (s->canRemove({ m })) {
		c->add(new DestroyCommand<Object, Scene>({ m }, s));
		return true;
	}
	return false;
}

void ObjectCommand::setMaterial(Commands* c, Object* o, Material* m) {
	addValueCommand(c, o, &Object::setMaterial, &Object::material, m);
}
} // namespace eno
