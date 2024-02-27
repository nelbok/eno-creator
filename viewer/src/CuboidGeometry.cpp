#include "CuboidGeometry.hpp"

#include <QtGui/QVector3D>

#include <eno/data/Material.hpp>
#include <eno/data/Object.hpp>
#include <eno/data/Scene.hpp>
#include <eno/tools/Geometry.hpp>

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
	QVector3D min{};
	QVector3D max{};
	for (auto* material : _project->materials()) {
		QVector3D minMat{};
		QVector3D maxMat{};

		indexOffset += indexCount;
		indexCount = 0u;

		for (Object* object : _project->scene()->objects()) {
			if (object->material() != material)
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
			if (indexCount) {
				minMat.setX(std::min(minMat.x(), pos.x()));
				minMat.setY(std::min(minMat.y(), pos.y()));
				minMat.setZ(std::min(minMat.z(), pos.z()));
				maxMat.setX(std::max(maxMat.x(), pos.x() + 1));
				maxMat.setY(std::max(maxMat.y(), pos.y() + 1));
				maxMat.setZ(std::max(maxMat.z(), pos.z() + 1));
			} else {
				minMat = pos;
				maxMat = pos;
			}

			arrayVertices += Geometry::createCuboidVertexData(object->position());
			arrayIndexes += Geometry::createCuboidIndexData(verticesCount);
			indexCount += 36; // 3 indices per triangle * 2 triangles per face * 6 faces per object
		}

		addSubset(indexOffset, indexCount, minMat, maxMat, material->name());
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

	update();
}
} // namespace eno
