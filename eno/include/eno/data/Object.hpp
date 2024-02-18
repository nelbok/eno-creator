#pragma once

#include <QtCore/QObject>
#include <QtGui/QVector3D>

namespace eno {
class Material;
class Project;

class Object : public QObject {

	Q_OBJECT
	Q_PROPERTY(QVector3D position READ position WRITE setPosition NOTIFY positionUpdated)
	Q_PROPERTY(Material* material READ material WRITE setMaterial NOTIFY materialUpdated)

public:
	Object(Project* project);
	virtual ~Object();

	// clang-format off
	const QVector3D& position() const { return _position; }
	void setPosition(const QVector3D& position);

	Material* material() const { return _material; }
	void setMaterial(Material* material);
	// clang-format on

private:
	Project* _project{ nullptr };

	QVector3D _position{};
	Material* _material{};

signals:
	void positionUpdated();
	void materialUpdated();
};
} // namespace eno
