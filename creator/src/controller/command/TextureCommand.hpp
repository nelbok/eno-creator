#pragma once

class QString;
class QPixmap;

namespace eno {
class Commands;
class Texture;
class Project;

struct TextureCommand {
	static Texture* create(Commands* commands, Project* project);
	static bool destroy(Commands* commands, Texture* texture, Project* project);
	static void setName(Commands* commands, Texture* texture, const QString& name);
	static void setData(Commands* c, Texture* t, const QPixmap& newValue);
};
} // namespace eno
