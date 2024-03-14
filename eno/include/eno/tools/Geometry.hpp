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
// 3 + 2 + 3 + 4
// vec + tex + nor + tan
quint32 stride();

// Faces of the cube
enum class PlaneNormal {
	None = 0,
	PositiveX = 1 << 1,
	NegativeX = 1 << 2,
	PositiveY = 1 << 3,
	NegativeY = 1 << 4,
	PositiveZ = 1 << 5,
	NegativeZ = 1 << 6,
	All = PositiveX | NegativeX | PositiveY | NegativeY | PositiveZ | NegativeZ,
};
constexpr auto operator*(PlaneNormal right) {
	return std::underlying_type_t<PlaneNormal>(right);
}
constexpr PlaneNormal operator|(PlaneNormal left, PlaneNormal right) {
	return static_cast<PlaneNormal>(*left | *right);
}
constexpr PlaneNormal operator^(PlaneNormal left, PlaneNormal right) {
	return static_cast<PlaneNormal>(*left ^ *right);
}
constexpr PlaneNormal operator&(PlaneNormal left, PlaneNormal right) {
	return static_cast<PlaneNormal>(*left & *right);
}

// Create a cuboid and gives back datas in function of the position
QByteArray createCuboidVertexData(const QVector3D& position, PlaneNormal faces = PlaneNormal::All);

// baseIndex is used when you are more than one cube in your mesh
QByteArray createCuboidIndexData(quint32& baseIndex, PlaneNormal faces = PlaneNormal::All);

} // namespace eno::Geometry
