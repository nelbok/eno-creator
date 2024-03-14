#pragma once

#include <QtCore/QList>
#include <QtGui/QVector3D>

#include <eno/tools/Geometry.hpp>

namespace eno {
class Material;
class Scene;

namespace Merger {
struct Cuboid {
	QVector3D position{ 0, 0, 0 };
	Geometry::PlaneNormal faces{ Geometry::PlaneNormal::All };
	Material* material{ nullptr };
};

QList<Cuboid> fillData(const Scene* scene);
QList<Cuboid> mergeData(const Scene* scene);
QList<Cuboid> mergeData(QList<Cuboid> data);
} // namespace Merger
} // namespace eno
