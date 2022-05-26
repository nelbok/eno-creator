#pragma once

#include <QWidget>

#include <QMap>
#include <QColor>

namespace Qt3DCore {
class QEntity;
class QComponent;
}
namespace Qt3DExtras {
class Qt3DWindow;
}

namespace eno {
class Data;

class Engine : public QWidget {
	Q_OBJECT

public:
	Engine(QWidget* parent = 0);
	virtual ~Engine() = default;

	void init(const Data* data);

private:
	void initCamera();
	void initLight();
	void initCube();

	Qt3DCore::QComponent* getMaterialBy(const QColor& color);

	Qt3DExtras::Qt3DWindow* _view{ nullptr };
	Qt3DCore::QEntity* _root{ nullptr };
	Qt3DCore::QComponent* _mesh{ nullptr };

	QMap<QColor, Qt3DCore::QComponent*> _materials{};
};
} // namespace eno
