#include "CuboidGeometry.hpp"

#include <QtGui/QVector3D>

#include <eno/data/Material.hpp>
#include <eno/data/Object.hpp>
#include <eno/data/Scene.hpp>
#include <eno/tools/Geometry.hpp>
#include <eno/tools/Utils.hpp>

namespace eno {

CuboidGeometry::CuboidGeometry(QQuick3DObject* parent)
	: QQuick3DGeometry(parent) {}

void CuboidGeometry::setProject(Project* project) {
	_project = project;
	updateData();
}

void CuboidGeometry::updateData() {
	clear();
	if (!_project) {
		update();
		return;
	}

	auto verticesCount = 0u;
	auto indexOffset = 0u;
	auto indexCount = 0u;
	QByteArray arrayVertices;
	QByteArray arrayIndexes;
	const auto& materials = _project->materials();
	const auto& objects = _project->scene()->objects();
	for (auto* material : materials) {
		indexOffset += indexCount;
		indexCount = 0u;

		QList<QVector3D> bb;
		for (Object* object : objects) {
			if (object->material() != material)
				continue;

			const auto& pos = object->position();
			bb.append(pos);
			arrayVertices += Geometry::createCuboidVertexData(pos);
			arrayIndexes += Geometry::createCuboidIndexData(verticesCount);
			indexCount += 36; // 3 indices per triangle * 2 triangles per face * 6 faces per object
		}

		auto [minMat, maxMat] = Utils::boundingBox(bb);
		addSubset(indexOffset, indexCount, minMat, maxMat, material->name());
	}

	setVertexData(arrayVertices);
	setIndexData(arrayIndexes);
	setStride(Geometry::stride());
	auto [min, max] = Utils::boundingBox(objects);
	setBounds(min, max);

	setPrimitiveType(QQuick3DGeometry::PrimitiveType::Triangles);

	addAttribute(QQuick3DGeometry::Attribute::PositionSemantic, 0, QQuick3DGeometry::Attribute::F32Type);
	addAttribute(QQuick3DGeometry::Attribute::TexCoordSemantic, 3 * sizeof(float), QQuick3DGeometry::Attribute::F32Type);
	addAttribute(QQuick3DGeometry::Attribute::NormalSemantic, 5 * sizeof(float), QQuick3DGeometry::Attribute::F32Type);
	addAttribute(QQuick3DGeometry::Attribute::TangentSemantic, 8 * sizeof(float), QQuick3DGeometry::Attribute::F32Type);
	addAttribute(QQuick3DGeometry::Attribute::IndexSemantic, 0, QQuick3DGeometry::Attribute::U32Type);

	update();
}
} // namespace eno
