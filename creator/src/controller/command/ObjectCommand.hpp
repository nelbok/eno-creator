#pragma once

namespace eno {
class Commands;
class Material;
class Object;
class Project;
class Scene;

struct ObjectCommand {
	static Object* create(Commands* commands, Project* project);
	static bool destroy(Commands* commands, Object* object, Scene* scene);
	static void setMaterial(Commands* commands, Object* object, Material* material);
};
} // namespace eno
