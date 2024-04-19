#include <eno/engine/CuboidGeometry.hpp>

#include <QtGui/QVector3D>

#include <eno/data/Material.hpp>
#include <eno/data/Object.hpp>
#include <eno/data/Scene.hpp>
#include <eno/tools/Geometry.hpp>
#include <eno/tools/Merger.hpp>
#include <eno/tools/Utils.hpp>

namespace eno {

CuboidGeometry::CuboidGeometry(QQuick3DObject* parent)
	: QQuick3DGeometry(parent) {}

CuboidGeometry::~CuboidGeometry() {
	clear();
}

void CuboidGeometry::setProject(Project* project) {
	_project = project;
	updateData();
}

void CuboidGeometry::setOptimized(bool optimized) {
	_optimized = optimized;
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

	// Optimized 3D view if needed
	auto objects = Merger::fillData(_project->scene());
	if (_optimized) {
		objects = Merger::mergeData(objects);
	}

	for (auto* material : materials) {
		indexOffset += indexCount;

		QByteArray arrayOneVertices;
		QByteArray arrayOneIndexes;

		QList<QVector3D> bb;
		for (const auto& object : objects) {
			if (object.material != material)
				continue;

			bb.append(object.position);
			arrayOneVertices += Geometry::createCuboidVertexData(object.position, object.faces);
			arrayOneIndexes += Geometry::createCuboidIndexData(verticesCount, object.faces);
		}

		auto [minMat, maxMat] = Utils::boundingBox(bb);
		indexCount = static_cast<unsigned int>(arrayOneIndexes.length()) / sizeof(quint32);
		addSubset(indexOffset, indexCount, minMat, maxMat, material->name());

		arrayVertices += arrayOneVertices;
		arrayIndexes += arrayOneIndexes;
	}

	setVertexData(arrayVertices);
	setIndexData(arrayIndexes);
	setStride(Geometry::stride());
	auto [min, max] = Utils::boundingBox(_project->scene()->objects());
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
