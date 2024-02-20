#pragma once

class QColor;
class QString;

namespace eno {
class Material;
class UndoRedo;

struct MaterialCommand {
	static void setName(UndoRedo* undoRedo, Material* material, const QString& name);
	static void setDiffuse(UndoRedo* undoRedo, Material* material, const QColor& color);
};
} // namespace eno
