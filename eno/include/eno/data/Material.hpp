#pragma once

#include <QtGui/QColor>

#include <eno/data/Container.hpp>

namespace eno {
class Project;

class Material : public Item {
	Q_OBJECT
	Q_PROPERTY(QUuid uuid READ uuid NOTIFY uuidUpdated)
	Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameUpdated)
	Q_PROPERTY(QColor diffuse READ diffuse WRITE setDiffuse NOTIFY diffuseUpdated)

public:
	Material(Project* project);
	Material(const QUuid& uuid, Project* project);
	virtual ~Material() = default;

	// clang-format off
	const QString& name() const { return _name; }
	void setName(const QString& name);

	const QColor& diffuse() const { return _diffuse; }
	void setDiffuse(const QColor& color);
	// clang-format on

private:
	QString _name{};
	QColor _diffuse{};

signals:
	void nameUpdated();
	void diffuseUpdated();
};
} // namespace eno
