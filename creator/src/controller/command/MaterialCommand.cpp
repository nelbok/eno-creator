#include "MaterialCommand.hpp"

#include <eno/data/Material.hpp>
#include <eno/data/Project.hpp>

#include "controller/UndoRedo.hpp"
#include "BaseCommand.hpp"

namespace eno {
Material* MaterialCommand::create(UndoRedo* u, Project* p) {
	auto* command = new CreateCommand<Material, Project>(p);
	u->add(command);
	return command->instance();
}


void MaterialCommand::setName(UndoRedo* u, Material* m, const QString& newValue) {
	u->add(new ValueCommand(m, &Material::setName, m->name(), newValue));
}

void MaterialCommand::setDiffuse(UndoRedo* u, Material* m, const QColor& newValue) {
	u->add(new ValueCommand(m, &Material::setDiffuse, m->diffuse(), newValue));
}
} // namespace eno
