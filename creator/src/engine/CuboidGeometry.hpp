#pragma once

#include <Qt3DExtras/qt3dextras_global.h>
#include <Qt3DCore/QGeometry>
#include <QtCore/QSize>

namespace Qt3DCore {
class QAttribute;
}
namespace eno {
class Material;
class Scene;

class CuboidGeometry : public Qt3DCore::QGeometry {
	Q_OBJECT

public:
	CuboidGeometry(Qt3DCore::QNode* parent = nullptr);
	virtual ~CuboidGeometry() = default;

	void init(const Scene* scene, const Material* material);

private:
	Qt3DCore::QAttribute* _positionAttribute{ nullptr };
	Qt3DCore::QAttribute* _normalAttribute{ nullptr };
	Qt3DCore::QAttribute* _texCoordAttribute{ nullptr };
	Qt3DCore::QAttribute* _tangentAttribute{ nullptr };
	Qt3DCore::QAttribute* _indexAttribute{ nullptr };
};
} // namespace eno
