#pragma once

#include <QtCore/QMap>
#include <QtWidgets/QWidget>

namespace Qt3DCore {
class QEntity;
class QComponent;
}
namespace Qt3DExtras {
class Qt3DWindow;
}

namespace eno {
class Project;
class Material;

class Engine : public QWidget {
	Q_OBJECT

public:
	Engine(QWidget* parent = 0);
	virtual ~Engine() = default;

	void init(const Project* scene);

private:
	void initCamera();
	void initLight();

	Qt3DCore::QComponent* createMaterialComponent(Material* color);

	Qt3DExtras::Qt3DWindow* _view{ nullptr };
	Qt3DCore::QEntity* _root{ nullptr };
};
} // namespace eno
