#include "MaterialCommand.hpp"

#include <eno/data/Material.hpp>
#include <eno/data/Project.hpp>

#include "controller/Commands.hpp"
#include "CreateCommand.hpp"
#include "DestroyCommand.hpp"
#include "ValueCommand.hpp"

namespace eno {
Material* MaterialCommand::create(Commands* c, Project* p) {
	auto* command = new CreateCommand<Material, Project, Project>(p, p, 1);
	c->add(command);
	return command->instances().first();
}

bool MaterialCommand::destroy(Commands* c, Material* m, Project* p) {
	if (p->canRemove({ m }) && p->materials().count() > 1) {
		c->add(new DestroyCommand<Material, Project>({ m }, p));
		return true;
	}
	return false;
}

void MaterialCommand::setName(Commands* c, Material* m, const QString& newValue) {
	addValueCommand(c, m, &Material::setName, &Material::name, newValue);
}

void MaterialCommand::setDiffuse(Commands* c, Material* m, const QColor& newValue) {
	addValueCommand(c, m, &Material::setDiffuse, &Material::diffuse, newValue);
}

void MaterialCommand::setTexture(Commands* c, Material* m, Texture* newValue) {
	addValueCommand(c, m, &Material::setTexture, &Material::texture, newValue);
}
} // namespace eno
