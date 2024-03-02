#include "CuboidGeometry.hpp"

#include <limits>

#include <QtCore/QSize>
#include <QtGui/QVector3D>

// See source: https://code.woboq.org/qt5/qt3d/src/extras/geometries/qcuboidgeometry.cpp.html

#include <Qt3DCore/QAttribute>
#include <Qt3DCore/QBuffer>

#include <eno/data/Object.hpp>
#include <eno/data/Scene.hpp>
#include <eno/tools/Geometry.hpp>

using namespace Qt3DCore;

namespace eno {
CuboidGeometry::CuboidGeometry(Qt3DCore::QNode* parent)
	: QGeometry(parent) {}

CuboidGeometry::~CuboidGeometry() {
	_positionAttribute->deleteLater();
	_normalAttribute->deleteLater();
	_texCoordAttribute->deleteLater();
	_tangentAttribute->deleteLater();
	_indexAttribute->deleteLater();

	_bufferVertices->deleteLater();
	_bufferIndexes->deleteLater();
}

void CuboidGeometry::init(const Scene* scene, const Material* material) {
	assert(scene);

	_positionAttribute = new QAttribute(this);
	_normalAttribute = new QAttribute(this);
	_texCoordAttribute = new QAttribute(this);
	_tangentAttribute = new QAttribute(this);
	_indexAttribute = new QAttribute(this);

	auto verticesCount = 0u;
	auto indexesCount = 0u;

	_bufferVertices = new QBuffer(this);
	_bufferIndexes = new QBuffer(this);
	QByteArray arrayVertices;
	QByteArray arrayIndexes;
	const auto& objects = scene->objects();
	for (Object* object : objects) {
		if (object->material() != material)
			continue;
		arrayVertices += Geometry::createCuboidVertexData(object->position());
		arrayIndexes += Geometry::createCuboidIndexData(verticesCount);
	}
	_bufferVertices->setData(arrayVertices);
	_bufferIndexes->setData(arrayIndexes);

	indexesCount = verticesCount * 3 / 2;

	_positionAttribute->setName(QAttribute::defaultPositionAttributeName());
	_positionAttribute->setVertexBaseType(QAttribute::Float);
	_positionAttribute->setVertexSize(3);
	_positionAttribute->setAttributeType(QAttribute::VertexAttribute);
	_positionAttribute->setBuffer(_bufferVertices);
	_positionAttribute->setByteStride(Geometry::stride());
	_positionAttribute->setCount(verticesCount);

	_texCoordAttribute->setName(QAttribute::defaultTextureCoordinateAttributeName());
	_texCoordAttribute->setVertexBaseType(QAttribute::Float);
	_texCoordAttribute->setVertexSize(2);
	_texCoordAttribute->setAttributeType(QAttribute::VertexAttribute);
	_texCoordAttribute->setBuffer(_bufferVertices);
	_texCoordAttribute->setByteStride(Geometry::stride());
	_texCoordAttribute->setByteOffset(3 * sizeof(float));
	_texCoordAttribute->setCount(verticesCount);

	_normalAttribute->setName(QAttribute::defaultNormalAttributeName());
	_normalAttribute->setVertexBaseType(QAttribute::Float);
	_normalAttribute->setVertexSize(3);
	_normalAttribute->setAttributeType(QAttribute::VertexAttribute);
	_normalAttribute->setBuffer(_bufferVertices);
	_normalAttribute->setByteStride(Geometry::stride());
	_normalAttribute->setByteOffset(5 * sizeof(float));
	_normalAttribute->setCount(verticesCount);

	_tangentAttribute->setName(QAttribute::defaultTangentAttributeName());
	_tangentAttribute->setVertexBaseType(QAttribute::Float);
	_tangentAttribute->setVertexSize(4);
	_tangentAttribute->setAttributeType(QAttribute::VertexAttribute);
	_tangentAttribute->setBuffer(_bufferVertices);
	_tangentAttribute->setByteStride(Geometry::stride());
	_tangentAttribute->setByteOffset(8 * sizeof(float));
	_tangentAttribute->setCount(verticesCount);

	_indexAttribute->setAttributeType(QAttribute::IndexAttribute);
	_indexAttribute->setVertexBaseType(QAttribute::UnsignedInt);
	_indexAttribute->setBuffer(_bufferIndexes);
	_indexAttribute->setCount(indexesCount);

	addAttribute(_positionAttribute);
	addAttribute(_texCoordAttribute);
	addAttribute(_normalAttribute);
	addAttribute(_tangentAttribute);
	addAttribute(_indexAttribute);
}

} // namespace eno
