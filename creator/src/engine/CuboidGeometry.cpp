#include "CuboidGeometry.hpp"

#include <limits>

#include <QtCore/QSize>
#include <QtGui/QVector3D>

// See source: https://code.woboq.org/qt5/qt3d/src/extras/geometries/qcuboidgeometry.cpp.html

#include <Qt3DCore/QAttribute>
#include <Qt3DCore/QBuffer>

#include <eno/data/Object.hpp>
#include <eno/data/Scene.hpp>

using namespace Qt3DCore;

namespace eno {
namespace detail {

enum PlaneNormal { PositiveX, NegativeX, PositiveY, NegativeY, PositiveZ, NegativeZ };

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

void createPlaneIndexData(unsigned int* indices, unsigned int& baseIndex) {
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

QByteArray createCuboidVertexData(const QVector3D& position) {
	const int yzVerts = 4;
	const int xzVerts = 4;
	const int xyVerts = 4;
	const int nVerts = 2 * (yzVerts + xzVerts + xyVerts);
	const quint32 elementSize = 3 + 3 + 2 + 4;
	const quint32 stride = elementSize * sizeof(float);
	QByteArray vertexBytes;
	vertexBytes.resize(stride * nVerts);
	float* vertices = reinterpret_cast<float*>(vertexBytes.data());
	createPlaneVertexData(PositiveX, position, vertices);
	vertices += yzVerts * elementSize;
	createPlaneVertexData(NegativeX, position, vertices);
	vertices += yzVerts * elementSize;
	createPlaneVertexData(PositiveY, position, vertices);
	vertices += xzVerts * elementSize;
	createPlaneVertexData(NegativeY, position, vertices);
	vertices += xzVerts * elementSize;
	createPlaneVertexData(PositiveZ, position, vertices);
	vertices += xyVerts * elementSize;
	createPlaneVertexData(NegativeZ, position, vertices);
	return vertexBytes;
}

QByteArray createCuboidIndexData(unsigned int& baseIndex) {
	const int yzIndices = 2 * 3;
	const int xzIndices = 2 * 3;
	const int xyIndices = 2 * 3;
	const int indexCount = 2 * (yzIndices + xzIndices + xyIndices);
	QByteArray indexData;
	indexData.resize(indexCount * sizeof(unsigned int));
	unsigned int* indices = reinterpret_cast<unsigned int*>(indexData.data());
	createPlaneIndexData(indices, baseIndex);
	indices += yzIndices;
	createPlaneIndexData(indices, baseIndex);
	indices += yzIndices;
	createPlaneIndexData(indices, baseIndex);
	indices += xzIndices;
	createPlaneIndexData(indices, baseIndex);
	indices += xzIndices;
	createPlaneIndexData(indices, baseIndex);
	indices += xyIndices;
	createPlaneIndexData(indices, baseIndex);
	return indexData;
}
} // namespace detail

CuboidGeometry::CuboidGeometry(Qt3DCore::QNode* parent)
	: QGeometry(parent) {}

void CuboidGeometry::init(const Scene* scene, const Material* material) {
	assert(scene);

	_positionAttribute = new QAttribute(this);
	_normalAttribute = new QAttribute(this);
	_texCoordAttribute = new QAttribute(this);
	_tangentAttribute = new QAttribute(this);
	_indexAttribute = new QAttribute(this);

	// vec3 pos vec2 tex vec3 normal vec4 tangent
	const quint32 stride = (3 + 2 + 3 + 4) * sizeof(float);

	auto verticesCount = 0u;
	auto indexesCount = 0u;

	auto* bufferVertices = new QBuffer(this);
	auto* bufferIndexes = new QBuffer(this);
	QByteArray arrayVertices;
	QByteArray arrayIndexes;
	for (Object* object : scene->objects()) {
		if (object->material() != material)
			continue;
		arrayVertices += detail::createCuboidVertexData(object->position());
		arrayIndexes += detail::createCuboidIndexData(verticesCount);
	}
	bufferVertices->setData(arrayVertices);
	bufferIndexes->setData(arrayIndexes);

	indexesCount = verticesCount * 3 / 2;

	_positionAttribute->setName(QAttribute::defaultPositionAttributeName());
	_positionAttribute->setVertexBaseType(QAttribute::Float);
	_positionAttribute->setVertexSize(3);
	_positionAttribute->setAttributeType(QAttribute::VertexAttribute);
	_positionAttribute->setBuffer(bufferVertices);
	_positionAttribute->setByteStride(stride);
	_positionAttribute->setCount(verticesCount);

	_texCoordAttribute->setName(QAttribute::defaultTextureCoordinateAttributeName());
	_texCoordAttribute->setVertexBaseType(QAttribute::Float);
	_texCoordAttribute->setVertexSize(2);
	_texCoordAttribute->setAttributeType(QAttribute::VertexAttribute);
	_texCoordAttribute->setBuffer(bufferVertices);
	_texCoordAttribute->setByteStride(stride);
	_texCoordAttribute->setByteOffset(3 * sizeof(float));
	_texCoordAttribute->setCount(verticesCount);

	_normalAttribute->setName(QAttribute::defaultNormalAttributeName());
	_normalAttribute->setVertexBaseType(QAttribute::Float);
	_normalAttribute->setVertexSize(3);
	_normalAttribute->setAttributeType(QAttribute::VertexAttribute);
	_normalAttribute->setBuffer(bufferVertices);
	_normalAttribute->setByteStride(stride);
	_normalAttribute->setByteOffset(5 * sizeof(float));
	_normalAttribute->setCount(verticesCount);

	_tangentAttribute->setName(QAttribute::defaultTangentAttributeName());
	_tangentAttribute->setVertexBaseType(QAttribute::Float);
	_tangentAttribute->setVertexSize(4);
	_tangentAttribute->setAttributeType(QAttribute::VertexAttribute);
	_tangentAttribute->setBuffer(bufferVertices);
	_tangentAttribute->setByteStride(stride);
	_tangentAttribute->setByteOffset(8 * sizeof(float));
	_tangentAttribute->setCount(verticesCount);

	_indexAttribute->setAttributeType(QAttribute::IndexAttribute);
	_indexAttribute->setVertexBaseType(QAttribute::UnsignedInt);
	_indexAttribute->setBuffer(bufferIndexes);
	_indexAttribute->setCount(indexesCount);

	addAttribute(_positionAttribute);
	addAttribute(_texCoordAttribute);
	addAttribute(_normalAttribute);
	addAttribute(_tangentAttribute);
	addAttribute(_indexAttribute);
}

} // namespace eno
