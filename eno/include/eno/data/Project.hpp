#pragma once

#include <QtCore/QObject>

#include <eno/data/Container.hpp>
#include <eno/data/Material.hpp>

// For Q_PROPERTY
#include <eno/data/Scene.hpp>

namespace eno {
class Project
	: public QObject
	, public Container<Material, Project> {
	Q_OBJECT
	Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameUpdated)
	Q_PROPERTY(QString filePath READ filePath WRITE setFilePath NOTIFY filePathUpdated)
	Q_PROPERTY(bool isModified READ isModified WRITE setIsModified NOTIFY isModifiedUpdated)
	Q_PROPERTY(Scene* scene READ scene CONSTANT)

public:
	Project(QObject* parent = nullptr);
	virtual ~Project();

	Q_INVOKABLE void init();
	Q_INVOKABLE void reset();

	// clang-format off
	const QString& name() const { return _name; }
	void setName(const QString& name);

	const QString& filePath() const { return _filePath; }
	void setFilePath(const QString& filePath);

	bool isModified() const { return _isModified; }
	void setIsModified(bool isModifed);

	inline QList<Material*> materials() const { return Container::datas(); }

	Scene* scene() { return _scene; }
	const Scene* scene() const { return _scene; }
	// clang-format on

private:
	virtual void datasUpdated() override;

	QString _name{};
	QString _filePath{};
	bool _isModified{ false };
	Scene* _scene{ nullptr };

signals:
	void nameUpdated();
	void filePathUpdated();
	void isModifiedUpdated();
	void materialsUpdated();
};
} // namespace eno
