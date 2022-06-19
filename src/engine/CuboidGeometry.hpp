#pragma once

#include <Qt3DExtras/qt3dextras_global.h>
#if QT_VERSION > QT_VERSION_CHECK(6, 0, 0)
#	include <Qt3DCore/QGeometry>
namespace Qt3DCore {
class QAttribute;
}
#else
#	include <Qt3DRender/QGeometry>
namespace Qt3DRender {
class QAttribute;
}
#endif
#include <QtCore/QSize>

namespace eno {
class Material;
class Scene;

#if QT_VERSION > QT_VERSION_CHECK(6, 0, 0)
class CuboidGeometry : public Qt3DCore::QGeometry {
#else
class CuboidGeometry : public Qt3DRender::QGeometry {
#endif
	Q_OBJECT

public:
	CuboidGeometry(Qt3DCore::QNode* parent = nullptr);
	virtual ~CuboidGeometry() = default;

	void init(const Scene* scene, const Material* material);

private:
#if QT_VERSION > QT_VERSION_CHECK(6, 0, 0)
	Qt3DCore::QAttribute* _positionAttribute{ nullptr };
	Qt3DCore::QAttribute* _normalAttribute{ nullptr };
	Qt3DCore::QAttribute* _texCoordAttribute{ nullptr };
	Qt3DCore::QAttribute* _tangentAttribute{ nullptr };
	Qt3DCore::QAttribute* _indexAttribute{ nullptr };
#else
	Qt3DRender::QAttribute* _positionAttribute{ nullptr };
	Qt3DRender::QAttribute* _normalAttribute{ nullptr };
	Qt3DRender::QAttribute* _texCoordAttribute{ nullptr };
	Qt3DRender::QAttribute* _tangentAttribute{ nullptr };
	Qt3DRender::QAttribute* _indexAttribute{ nullptr };
#endif
};
} // namespace eno
