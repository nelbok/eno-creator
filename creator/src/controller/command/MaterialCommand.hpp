#pragma once

class QColor;
class QString;

namespace eno {
class Material;
class Project;
class UndoRedo;

struct MaterialCommand {
	static Material* create(UndoRedo* undoRedo, Project* project);
	static void setName(UndoRedo* undoRedo, Material* material, const QString& name);
	static void setDiffuse(UndoRedo* undoRedo, Material* material, const QColor& color);
};
} // namespace eno
