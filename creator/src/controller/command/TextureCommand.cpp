#include "TextureCommand.hpp"

#include <eno/data/Texture.hpp>
#include <eno/data/Project.hpp>

#include "controller/Commands.hpp"
#include "CreateCommand.hpp"
#include "DestroyCommand.hpp"
#include "ValueCommand.hpp"

namespace eno {
Texture* TextureCommand::create(Commands* c, Project* p) {
	auto* command = new CreateCommand<Texture, Project, Project>(p, p, 1);
	c->add(command);
	return command->instances().first();
}

bool TextureCommand::destroy(Commands* c, Texture* t, Project* p) {
	if (p->canRemove({ t })) {
		c->add(new DestroyCommand<Texture, Project>({ t }, p));
		return true;
	}
	return false;
}

void TextureCommand::setName(Commands* c, Texture* t, const QString& newValue) {
	addValueCommand(c, t, &Texture::setName, &Texture::name, newValue);
}
} // namespace eno
