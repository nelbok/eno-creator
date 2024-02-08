#pragma once

#include <QVector3D>
#include <QList>

namespace eno {
class Material;
class Scene;
struct Cuboid {
	QVector3D position{ 0, 0, 0 };
	QVector3D scale{ 1, 1, 1 };
	Material* material{ nullptr };
};

QList<Cuboid> fillData(Scene* scene);
QList<Cuboid> mergeData(Scene* scene);
QList<Cuboid> mergeData(QList<Cuboid> data);

} // namespace eno
