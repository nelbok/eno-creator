#pragma once

#include <QObject>

#include <QColor>

namespace eno {
class Project;

class Material : public QObject {
	Q_OBJECT

public:
	Material(Project* project);
	virtual ~Material() = default;

	// clang-format off
	const QString& name() const { return _name; }
	void setName(const QString& name);

	const QColor& diffuse() const { return _diffuse; }
	void setDiffuse(const QColor& color);

	int refCount() const { return _refCount; }
	void increaseRefCount();
	void decreaseRefCount();
	// clang-format on

private:
	Project* _project{ nullptr };

	QString _name{ "default" };
	QColor _diffuse{ "#ffaa00" };
	int _refCount{ 0 };

signals:
	void nameUpdated();
	void diffuseUpdated();
	void refCountUpdated();
};
} // namespace eno
