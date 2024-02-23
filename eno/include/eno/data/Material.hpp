#pragma once

#include <QtCore/QObject>
#include <QtCore/QUuid>
#include <QtGui/QColor>

#include <eno/data/Container.hpp>

namespace eno {
class Project;

class Material : public Item {
	friend class Eno;

	Q_OBJECT
	Q_PROPERTY(QUuid uuid READ uuid NOTIFY uuidUpdated)
	Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameUpdated)
	Q_PROPERTY(QColor diffuse READ diffuse WRITE setDiffuse NOTIFY diffuseUpdated)

public:
	Material(Project* project);
	virtual ~Material() = default;

	// clang-format off
	const QUuid& uuid() const { return _uuid; }

	const QString& name() const { return _name; }
	void setName(const QString& name);

	const QColor& diffuse() const { return _diffuse; }
	void setDiffuse(const QColor& color);
	// clang-format on

private:
	Project* _project{ nullptr };

	QUuid _uuid{ QUuid::createUuid() };
	QString _name{};
	QColor _diffuse{};

signals:
	void uuidUpdated();
	void nameUpdated();
	void diffuseUpdated();
};
} // namespace eno
