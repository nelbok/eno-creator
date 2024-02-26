#pragma once

#include <QtCore/QByteArray>

class QVector3D;

namespace eno::Geometry {
// Cuboid data are stored in this order:
// - vertices (float)
// - textures (float)
// - normals (float)
// - tangent (float)

// Indices (quint32)

// As explained above, stride will be:
// 3 + 3 + 2 + 4
// vec + tex + nor + tan
quint32 stride();

// Create a cuboid and gives back datas in function of the position
QByteArray createCuboidVertexData(const QVector3D& position);

// baseIndex is used when you are more than one cube in your mesh
QByteArray createCuboidIndexData(quint32& baseIndex);
} // namespace eno::Geometry
