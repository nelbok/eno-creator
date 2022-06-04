#pragma once

#include <QWidget>

#include <QMap>

namespace Qt3DCore {
class QEntity;
class QComponent;
}
namespace Qt3DExtras {
class Qt3DWindow;
}

namespace eno {
class Scene;
class Material;

class Engine : public QWidget {
	Q_OBJECT

public:
	Engine(QWidget* parent = 0);
	virtual ~Engine() = default;

	void init(const Scene* scene);

private:
	void initCamera();
	void initLight();
	void initCube();

	Qt3DCore::QComponent* getMaterialBy(Material* color);

	Qt3DExtras::Qt3DWindow* _view{ nullptr };
	Qt3DCore::QEntity* _root{ nullptr };
	Qt3DCore::QComponent* _mesh{ nullptr };

	QMap<Material*, Qt3DCore::QComponent*> _materials{};
};
} // namespace eno
