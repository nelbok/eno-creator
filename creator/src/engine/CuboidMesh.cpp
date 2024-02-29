#include "CuboidMesh.hpp"

#include "CuboidGeometry.hpp"

namespace eno {

CuboidMesh::CuboidMesh(QNode* parent)
	: QGeometryRenderer(parent) {}

CuboidMesh::~CuboidMesh() {
	_geometry->deleteLater();
}


void CuboidMesh::init(const Scene* scene, const Material* material) {
	_geometry = new CuboidGeometry(this);
	_geometry->init(scene, material);
	QGeometryRenderer::setGeometry(_geometry);
}

} // namespace eno
