#include "CuboidMesh.hpp"

#include "CuboidGeometry.hpp"

namespace eno {

CuboidMesh::CuboidMesh(QNode* parent)
	: QGeometryRenderer(parent) {}

void CuboidMesh::init(const Scene* scene, const Material* material) {
	auto* geometry = new CuboidGeometry(this);
	geometry->init(scene, material);
	QGeometryRenderer::setGeometry(geometry);
}

} // namespace eno
