#include <eno/tools/Geometry.hpp>

// See source: https://code.woboq.org/qt5/qt3d/src/extras/geometries/qcuboidgeometry.cpp.html

#include <QtGui/QVector3D>

namespace eno::Geometry {
namespace detail {
enum PlaneNormal { PositiveX, NegativeX, PositiveY, NegativeY, PositiveZ, NegativeZ };

// vertex + texture + normal + tangent
constexpr quint32 elementSize = 3 + 2 + 3 + 4;

void createPlaneVertexData(PlaneNormal normal, const QVector3D& position, float* vertices) {
	switch (normal) {
		case NegativeX:
			// Iterate over z
			for (int j = 1; j >= 0; --j) {
				// Iterate over y
				for (int i = 0; i < 2; ++i) {
					// position
					*vertices++ = position.x() - 0.5f;
					*vertices++ = position.y() - 0.5f + static_cast<float>(i);
					*vertices++ = position.z() - 0.5f + static_cast<float>(j);
					// texture coordinates
					*vertices++ = static_cast<float>(j);
					*vertices++ = static_cast<float>(i);
					// normal
					*vertices++ = -1.0f;
					*vertices++ = 0.0f;
					*vertices++ = 0.0f;
					// tangent
					*vertices++ = 0.0f;
					*vertices++ = 0.0f;
					*vertices++ = 1.0f;
					*vertices++ = 1.0f;
				}
			}
			break;
		case PositiveX: {
			// Iterate over z
			for (int j = 0; j < 2; ++j) {
				// Iterate over y
				for (int i = 0; i < 2; ++i) {
					// position
					*vertices++ = position.x() + 0.5f;
					*vertices++ = position.y() - 0.5f + static_cast<float>(i);
					*vertices++ = position.z() - 0.5f + static_cast<float>(j);
					// texture coordinates
					*vertices++ = 1.0f - static_cast<float>(j);
					*vertices++ = static_cast<float>(i);
					// normal
					*vertices++ = 1.0f;
					*vertices++ = 0.0f;
					*vertices++ = 0.0f;
					// tangent
					*vertices++ = 0.0f;
					*vertices++ = 0.0f;
					*vertices++ = -1.0f;
					*vertices++ = 1.0f;
				}
			}
			break;
		}
		case NegativeY:
			// Iterate over z
			for (int j = 0; j < 2; ++j) {
				// Iterate over x
				// This iterates in the opposite sense to the other directions
				// so that the winding order is correct
				for (int i = 0; i < 2; ++i) {
					// position
					*vertices++ = position.x() - 0.5f + static_cast<float>(i);
					*vertices++ = position.y() - 0.5f;
					*vertices++ = position.z() - 0.5f + static_cast<float>(j);
					// texture coordinates
					*vertices++ = static_cast<float>(i);
					*vertices++ = static_cast<float>(j);
					// normal
					*vertices++ = 0.0f;
					*vertices++ = -1.0f;
					*vertices++ = 0.0f;
					// tangent
					*vertices++ = 1.0f;
					*vertices++ = 0.0f;
					*vertices++ = 0.0f;
					*vertices++ = 1.0f;
				}
			}
			break;
		case PositiveY: {
			// Iterate over z
			for (int j = 1; j >= 0; --j) {
				// Iterate over x
				// This iterates in the opposite sense to the other directions
				// so that the winding order is correct
				for (int i = 0; i < 2; ++i) {
					// position
					*vertices++ = position.x() - 0.5f + static_cast<float>(i);
					*vertices++ = position.y() + 0.5f;
					*vertices++ = position.z() - 0.5f + static_cast<float>(j);
					// texture coordinates
					*vertices++ = static_cast<float>(i);
					*vertices++ = 1.0f - static_cast<float>(j);
					// normal
					*vertices++ = 0.0f;
					*vertices++ = 1.0f;
					*vertices++ = 0.0f;
					// tangent
					*vertices++ = 1.0f;
					*vertices++ = 0.0f;
					*vertices++ = 0.0f;
					*vertices++ = 1.0f;
				}
			}
			break;
		}
		case NegativeZ:
			// Iterate over y
			for (int j = 0; j < 2; ++j) {
				// Iterate over x
				for (int i = 1; i >= 0; --i) {
					// position
					*vertices++ = position.x() - 0.5f + static_cast<float>(i);
					*vertices++ = position.y() - 0.5f + static_cast<float>(j);
					*vertices++ = position.z() - 0.5f;
					// texture coordinates
					*vertices++ = 1.0f - static_cast<float>(i);
					*vertices++ = static_cast<float>(j);
					// normal
					*vertices++ = 0.0f;
					*vertices++ = 0.0f;
					*vertices++ = -1.0f;
					// tangent
					*vertices++ = -1.0f;
					*vertices++ = 0.0f;
					*vertices++ = 0.0f;
					*vertices++ = 1.0f;
				}
			}
			break;
		case PositiveZ: {
			// Iterate over y
			for (int j = 0; j < 2; ++j) {
				// Iterate over x
				for (int i = 0; i < 2; ++i) {
					// position
					*vertices++ = position.x() - 0.5f + static_cast<float>(i);
					*vertices++ = position.y() - 0.5f + static_cast<float>(j);
					*vertices++ = position.z() + 0.5f;
					// texture coordinates
					*vertices++ = static_cast<float>(i);
					*vertices++ = static_cast<float>(j);
					// normal
					*vertices++ = 0.0f;
					*vertices++ = 0.0f;
					*vertices++ = 1.0f;
					// tangent
					*vertices++ = 1.0f;
					*vertices++ = 0.0f;
					*vertices++ = 0.0f;
					*vertices++ = 1.0f;
				}
			}
			break;
		}
	} // switch (normal)
}

void createPlaneIndexData(quint32* indices, quint32& baseIndex) {
	// Populate indices taking care to get correct CCW winding on all faces
	// Iterate over v direction (rows)
	for (int j = 0; j < 1; ++j) {
		const auto rowStartIndex = j * 2 + baseIndex;
		const auto nextRowStartIndex = (j + 1) * 2 + baseIndex;
		// Iterate over u direction (columns)
		for (int i = 0; i < 1; ++i) {
			// Split quad into two triangles
			*indices++ = rowStartIndex + i;
			*indices++ = rowStartIndex + i + 1;
			*indices++ = nextRowStartIndex + i;
			*indices++ = nextRowStartIndex + i;
			*indices++ = rowStartIndex + i + 1;
			*indices++ = nextRowStartIndex + i + 1;
		}
	}
	baseIndex += 4;
}
} // namespace detail

quint32 stride() {
	return detail::elementSize * sizeof(float);
}

QByteArray createCuboidVertexData(const QVector3D& position) {
	const int yzVerts = 4;
	const int xzVerts = 4;
	const int xyVerts = 4;
	const int nVerts = 2 * (yzVerts + xzVerts + xyVerts);
	QByteArray vertexBytes;
	vertexBytes.resize(stride() * nVerts);
	float* vertices = reinterpret_cast<float*>(vertexBytes.data());
	detail::createPlaneVertexData(detail::PositiveX, position, vertices);
	vertices += yzVerts * detail::elementSize;
	detail::createPlaneVertexData(detail::NegativeX, position, vertices);
	vertices += yzVerts * detail::elementSize;
	detail::createPlaneVertexData(detail::PositiveY, position, vertices);
	vertices += xzVerts * detail::elementSize;
	detail::createPlaneVertexData(detail::NegativeY, position, vertices);
	vertices += xzVerts * detail::elementSize;
	detail::createPlaneVertexData(detail::PositiveZ, position, vertices);
	vertices += xyVerts * detail::elementSize;
	detail::createPlaneVertexData(detail::NegativeZ, position, vertices);
	return vertexBytes;
}

QByteArray createCuboidIndexData(quint32& baseIndex) {
	const int yzIndices = 2 * 3;
	const int xzIndices = 2 * 3;
	const int xyIndices = 2 * 3;
	const int indexCount = 2 * (yzIndices + xzIndices + xyIndices);
	QByteArray indexData;
	indexData.resize(indexCount * sizeof(quint32));
	quint32* indices = reinterpret_cast<quint32*>(indexData.data());
	detail::createPlaneIndexData(indices, baseIndex);
	indices += yzIndices;
	detail::createPlaneIndexData(indices, baseIndex);
	indices += yzIndices;
	detail::createPlaneIndexData(indices, baseIndex);
	indices += xzIndices;
	detail::createPlaneIndexData(indices, baseIndex);
	indices += xzIndices;
	detail::createPlaneIndexData(indices, baseIndex);
	indices += xyIndices;
	detail::createPlaneIndexData(indices, baseIndex);
	return indexData;
}

} // namespace eno::Geometry
