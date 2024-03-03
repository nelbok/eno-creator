#pragma once

#include <QtGui/QVector3D>

#include <eno/data/Container.hpp>

// For Q_PROPERTY
#include <eno/data/Material.hpp>

namespace eno {
class Project;

class Object : public Item {

	Q_OBJECT
	Q_PROPERTY(QVector3D position READ position WRITE setPosition NOTIFY positionUpdated)
	Q_PROPERTY(Material* material READ material WRITE setMaterial NOTIFY materialUpdated)

public:
	Object(Project* project);
	Object(const QUuid& uuid, Project* project);
	virtual ~Object();

	// clang-format off
	const QVector3D& position() const { return _position; }
	void setPosition(const QVector3D& position);

	Material* material() const { return _material; }
	void setMaterial(Material* material);
	// clang-format on

private:
	QVector3D _position{};
	Material* _material{};

signals:
	void positionUpdated();
	void materialUpdated();
};
} // namespace eno
