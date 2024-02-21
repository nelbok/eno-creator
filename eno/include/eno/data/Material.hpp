#pragma once

#include <QtCore/QObject>
#include <QtCore/QUuid>
#include <QtGui/QColor>

namespace eno {
class Project;

class Material : public QObject {
	friend class Eno;

	Q_OBJECT
	Q_PROPERTY(QUuid uuid READ uuid NOTIFY uuidUpdated)
	Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameUpdated)
	Q_PROPERTY(QColor diffuse READ diffuse WRITE setDiffuse NOTIFY diffuseUpdated)
	Q_PROPERTY(int refCount READ refCount NOTIFY refCountUpdated)

public:
	Material(Project* project);
	virtual ~Material();

	// clang-format off
	const QUuid& uuid() const { return _uuid; }

	const QString& name() const { return _name; }
	void setName(const QString& name);

	const QColor& diffuse() const { return _diffuse; }
	void setDiffuse(const QColor& color);

	int refCount() const { return _refCount; }
	void increaseRefCount();
	void decreaseRefCount();

	bool isAlive() const { return _isAlive; }
	void setIsAlive(bool isAlive);
	// clang-format on

private:
	Project* _project{ nullptr };

	QUuid _uuid{ QUuid::createUuid() };
	QString _name{};
	QColor _diffuse{};
	int _refCount{ 0 };
	bool _isAlive{ true };

signals:
	void uuidUpdated();
	void nameUpdated();
	void diffuseUpdated();
	void refCountUpdated();
};
} // namespace eno
