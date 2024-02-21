#pragma once

#include <QtCore/QObject>

// For Q_PROPERTY
#include <eno/data/Scene.hpp>

namespace eno {
class Material;

class Project : public QObject {
	Q_OBJECT
	Q_PROPERTY(QString filePath READ filePath WRITE setFilePath NOTIFY filePathUpdated)
	Q_PROPERTY(bool isModified READ isModified WRITE setIsModified NOTIFY isModifiedUpdated)
	Q_PROPERTY(Scene* scene READ scene CONSTANT)

public:
	Project(QObject* parent = nullptr);
	virtual ~Project();

	void init();
	void reset();

	QString projectName() const;

	// clang-format off
	const QString& filePath() const { return _filePath; }
	void setFilePath(const QString& path);

	bool isModified() const { return _isModified; }
	void setIsModified(bool isModifed);

	void add(Material* material);
	bool canRemove(Material* material);
	void remove(Material* material);
	QList<Material*> materials() const;

	Scene* scene() { return _scene; }
	const Scene* scene() const { return _scene; }
	// clang-format on

private:
	QString _filePath{};
	bool _isModified{ false };

	QList<Material*> _materials{};
	Scene* _scene{ nullptr };

signals:
	void filePathUpdated();
	void isModifiedUpdated();
	void materialsUpdated();
};
} // namespace eno
