#pragma once

class QColor;
class QString;

namespace eno {
class Commands;
class Material;
class Project;
class Texture;

struct MaterialCommand {
	static Material* create(Commands* commands, Project* project);
	static bool destroy(Commands* commands, Material* material, Project* project);
	static void setName(Commands* commands, Material* material, const QString& name);
	static void setDiffuse(Commands* commands, Material* material, const QColor& color);
	static void setDiffuseMap(Commands* commands, Material* material, Texture* texture);
};
} // namespace eno
