#pragma once

#include <QtCore/QList>
#include <QtGui/QVector3D>

class QPoint;

namespace eno {
class Commands;
class Material;
class Object;
class Project;
class Scene;

struct SceneCommand {
	static bool add(Commands* commands, Project* project, const QList<QVector3D>& vecs, Material* material);
	static bool remove(Commands* commands, Scene* scene, const QList<QVector3D>& vecs);
	static bool remove(Commands* commands, Scene* scene, const QList<Object*>& objects);
	static bool resize(Commands* commands, Scene* scene, const QPoint& min, const QPoint& max);
};
} // namespace eno
