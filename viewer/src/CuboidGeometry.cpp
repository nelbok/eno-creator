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
	QVector3D min{};
	QVector3D max{};
	for (Object* object : _scene->objects()) {
		if (object->material() != _material)
			continue;

		const auto& pos = object->position();
		if (verticesCount) {
			min.setX(std::min(min.x(), pos.x()));
			min.setY(std::min(min.y(), pos.y()));
			min.setZ(std::min(min.z(), pos.z()));
			max.setX(std::max(max.x(), pos.x() + 1));
			max.setY(std::max(max.y(), pos.y() + 1));
			max.setZ(std::max(max.z(), pos.z() + 1));
		} else {
			min = pos;
			max = pos;
		}

		arrayVertices += Geometry::createCuboidVertexData(object->position());
		arrayIndexes += Geometry::createCuboidIndexData(verticesCount);
	}

	setVertexData(arrayVertices);
	setIndexData(arrayIndexes);
	setStride(Geometry::stride());
	setBounds(min, max);

	setPrimitiveType(QQuick3DGeometry::PrimitiveType::Triangles);

	addAttribute(QQuick3DGeometry::Attribute::PositionSemantic, 0, QQuick3DGeometry::Attribute::F32Type);
	addAttribute(QQuick3DGeometry::Attribute::TexCoordSemantic, 3 * sizeof(float), QQuick3DGeometry::Attribute::F32Type);
	addAttribute(QQuick3DGeometry::Attribute::NormalSemantic, 5 * sizeof(float), QQuick3DGeometry::Attribute::F32Type);
	addAttribute(QQuick3DGeometry::Attribute::TangentSemantic, 8 * sizeof(float), QQuick3DGeometry::Attribute::F32Type);
	addAttribute(QQuick3DGeometry::Attribute::IndexSemantic, 0, QQuick3DGeometry::Attribute::U32Type);
}
} // namespace eno
