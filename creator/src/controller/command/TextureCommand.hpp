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
	static void setData(Commands* commands, Texture* texture, const QPixmap& pixmap);
	static void setInvertX(Commands* commands, Texture* texture, bool invertX);
	static void setInvertY(Commands* commands, Texture* texture, bool invertY);
};
} // namespace eno
