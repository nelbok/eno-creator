#pragma once

#include <Qt3DRender/QGeometryRenderer>

namespace eno {
class Material;
class Scene;

class CuboidMesh : public Qt3DRender::QGeometryRenderer {
	Q_OBJECT

public:
	CuboidMesh(Qt3DCore::QNode* parent = nullptr);
	virtual ~CuboidMesh() = default;

	void init(const Scene* scene, const Material* material);
};
} // namespace eno
