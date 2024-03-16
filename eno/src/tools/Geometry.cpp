#include <eno/tools/Geometry.hpp>

// See source: https://code.woboq.org/qt5/qt3d/src/extras/geometries/qcuboidgeometry.cpp.html

#include <QtGui/QVector3D>

namespace eno::Geometry {
namespace detail {
// vertex + texture + normal + tangent
constexpr quint32 elementSize = 3 + 2 + 3 + 4;

void createPlaneVertexData(PlaneNormal normal, const QVector3D& position, float* vertices) {
	switch (normal) {
		case PlaneNormal::NegativeX:
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
		case PlaneNormal::PositiveX: {
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
		case PlaneNormal::NegativeY:
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
		case PlaneNormal::PositiveY: {
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
		case PlaneNormal::NegativeZ:
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
		case PlaneNormal::PositiveZ: {
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
		default:
			assert(false);
	} // switch (normal)
}

void createPlaneIndexData(quint32* indices, quint32& baseIndex) {
	// Populate indices taking care to get correct CCW winding on all faces
	*indices++ = baseIndex;
	*indices++ = baseIndex + 1;
	*indices++ = baseIndex + 2;
	*indices++ = baseIndex + 2;
	*indices++ = baseIndex + 1;
	*indices++ = baseIndex + 3;
	baseIndex += 4;
}

int nbVisibleFaces(PlaneNormal faces) {
	int count = 0;
	if ((*faces & *PlaneNormal::PositiveX) == *PlaneNormal::PositiveX)
		++count;
	if ((*faces & *PlaneNormal::NegativeX) == *PlaneNormal::NegativeX)
		++count;
	if ((*faces & *PlaneNormal::PositiveY) == *PlaneNormal::PositiveY)
		++count;
	if ((*faces & *PlaneNormal::NegativeY) == *PlaneNormal::NegativeY)
		++count;
	if ((*faces & *PlaneNormal::PositiveZ) == *PlaneNormal::PositiveZ)
		++count;
	if ((*faces & *PlaneNormal::NegativeZ) == *PlaneNormal::NegativeZ)
		++count;
	return count;
}
} // namespace detail

quint32 stride() {
	return detail::elementSize * sizeof(float);
}

QByteArray createCuboidVertexData(const QVector3D& position, PlaneNormal faces) {
	constexpr int nbVerticesByFace = 4;
	int verticeCount = detail::nbVisibleFaces(faces) * nbVerticesByFace;
	QByteArray vertexBytes;
	vertexBytes.resize(verticeCount * stride());
	float* vertices = reinterpret_cast<float*>(vertexBytes.data());
	if ((*faces & *PlaneNormal::PositiveX) == *PlaneNormal::PositiveX) {
		detail::createPlaneVertexData(PlaneNormal::PositiveX, position, vertices);
		vertices += nbVerticesByFace * detail::elementSize;
	}
	if ((*faces & *PlaneNormal::NegativeX) == *PlaneNormal::NegativeX) {
		detail::createPlaneVertexData(PlaneNormal::NegativeX, position, vertices);
		vertices += nbVerticesByFace * detail::elementSize;
	}
	if ((*faces & *PlaneNormal::PositiveY) == *PlaneNormal::PositiveY) {
		detail::createPlaneVertexData(PlaneNormal::PositiveY, position, vertices);
		vertices += nbVerticesByFace * detail::elementSize;
	}
	if ((*faces & *PlaneNormal::NegativeY) == *PlaneNormal::NegativeY) {
		detail::createPlaneVertexData(PlaneNormal::NegativeY, position, vertices);
		vertices += nbVerticesByFace * detail::elementSize;
	}
	if ((*faces & *PlaneNormal::PositiveZ) == *PlaneNormal::PositiveZ) {
		detail::createPlaneVertexData(PlaneNormal::PositiveZ, position, vertices);
		vertices += nbVerticesByFace * detail::elementSize;
	}
	if ((*faces & *PlaneNormal::NegativeZ) == *PlaneNormal::NegativeZ) {
		detail::createPlaneVertexData(PlaneNormal::NegativeZ, position, vertices);
		vertices += nbVerticesByFace * detail::elementSize;
	}
	return vertexBytes;
}

QByteArray createCuboidIndexData(quint32& baseIndex, PlaneNormal faces) {
	constexpr int nbIndicesByFace = 2 * 3;
	int indiceCount = detail::nbVisibleFaces(faces) * nbIndicesByFace;
	QByteArray indexData;
	indexData.resize(indiceCount * sizeof(quint32));
	quint32* indices = reinterpret_cast<quint32*>(indexData.data());
	if ((*faces & *PlaneNormal::PositiveX) == *PlaneNormal::PositiveX) {
		detail::createPlaneIndexData(indices, baseIndex);
		indices += nbIndicesByFace;
	}
	if ((*faces & *PlaneNormal::NegativeX) == *PlaneNormal::NegativeX) {
		detail::createPlaneIndexData(indices, baseIndex);
		indices += nbIndicesByFace;
	}
	if ((*faces & *PlaneNormal::PositiveY) == *PlaneNormal::PositiveY) {
		detail::createPlaneIndexData(indices, baseIndex);
		indices += nbIndicesByFace;
	}
	if ((*faces & *PlaneNormal::NegativeY) == *PlaneNormal::NegativeY) {
		detail::createPlaneIndexData(indices, baseIndex);
		indices += nbIndicesByFace;
	}
	if ((*faces & *PlaneNormal::PositiveZ) == *PlaneNormal::PositiveZ) {
		detail::createPlaneIndexData(indices, baseIndex);
		indices += nbIndicesByFace;
	}
	if ((*faces & *PlaneNormal::NegativeZ) == *PlaneNormal::NegativeZ) {
		detail::createPlaneIndexData(indices, baseIndex);
		indices += nbIndicesByFace;
	}
	return indexData;
}

} // namespace eno::Geometry
