#include "CuboidGeometry.hpp"

#include <QtGui/QVector3D>

#include <eno/data/Object.hpp>
#include <eno/tools/Geometry.hpp>

namespace eno {

CuboidGeometry::CuboidGeometry(QQuick3DObject* parent)
	: QQuick3DGeometry(parent) {}

void CuboidGeometry::setMaterial(Material* material) {
	_material = material;
	updateData();
}

void CuboidGeometry::setScene(Scene* scene) {
	_scene = scene;
	updateData();
}

void CuboidGeometry::updateData() {
	if (!_material)
		return;
	if (!_scene)
		return;

	clear();

	auto verticesCount = 0u;
	QByteArray arrayVertices;
	QByteArray arrayIndexes;
	for (Object* object : _scene->objects()) {
		if (object->material() != _material)
			continue;
		arrayVertices += Geometry::createCuboidVertexData(object->position());
		arrayIndexes += Geometry::createCuboidIndexData(verticesCount);
	}

	setVertexData(arrayVertices);
	setIndexData(arrayIndexes);
	setStride(Geometry::stride());
	setBounds(QVector3D(-1.0f, -1.0f, 0.0f), QVector3D(+1.0f, +1.0f, 0.0f));

	setPrimitiveType(QQuick3DGeometry::PrimitiveType::Triangles);

	addAttribute(QQuick3DGeometry::Attribute::PositionSemantic, 0, QQuick3DGeometry::Attribute::F32Type);
	addAttribute(QQuick3DGeometry::Attribute::TexCoordSemantic, 3 * sizeof(float), QQuick3DGeometry::Attribute::F32Type);
	addAttribute(QQuick3DGeometry::Attribute::NormalSemantic, 5 * sizeof(float), QQuick3DGeometry::Attribute::F32Type);
	addAttribute(QQuick3DGeometry::Attribute::TangentSemantic, 8 * sizeof(float), QQuick3DGeometry::Attribute::F32Type);
	addAttribute(QQuick3DGeometry::Attribute::IndexSemantic, 0, QQuick3DGeometry::Attribute::U32Type);
}
} // namespace eno
