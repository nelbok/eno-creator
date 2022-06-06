#pragma once

#include <QObject>

#include <QList>

namespace eno {
class Material;
class Project;

class Materials : public QObject {
	Q_OBJECT

public:
	Materials(Project* project);
	virtual ~Materials() = default;

	void reset();

	Material* createMaterial() const;
	void add(Material* material);
	bool canRemove(Material* material);
	void remove(Material* material);
	int count() const;

	QList<Material*>::const_iterator begin() const;
	QList<Material*>::const_iterator end() const;

private:
	Project* _project{ nullptr };

	QList<Material*> _materials{};

signals:
	void materialUpdated();
	void updated();
};
} // namespace eno
