#pragma once

#include <QtCore/QObject>
#include <QtCore/QStringList>

#include <eno/data/Container.hpp>
#include <eno/data/Material.hpp>
#include <eno/data/Texture.hpp>

// For Q_PROPERTY
#include <eno/data/Scene.hpp>

namespace eno {
class Project
	: public QObject
	, public Container<Material, Project>
	, public Container<Texture, Project> {
	Q_OBJECT
	Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameUpdated)
	Q_PROPERTY(QString filePath READ filePath WRITE setFilePath NOTIFY filePathUpdated)
	Q_PROPERTY(bool isModified READ isModified WRITE setIsModified NOTIFY isModifiedUpdated)
	Q_PROPERTY(QStringList tags READ tags NOTIFY tagsUpdated)
	Q_PROPERTY(QList<Material*> materials READ materials NOTIFY materialsUpdated)
	Q_PROPERTY(QList<Texture*> textures READ textures NOTIFY texturesUpdated)
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

	void add(const QStringList& tags);
	void remove(const QStringList& tags);
	QStringList tags() const { return _tags; }

	using Container<Material, Project>::add;
	using Container<Material, Project>::canRemove;
	using Container<Material, Project>::remove;
	inline QList<Material*> materials() const { return Container<Material, Project>::datas(); }

	using Container<Texture, Project>::add;
	using Container<Texture, Project>::canRemove;
	using Container<Texture, Project>::remove;
	inline QList<Texture*> textures() const { return Container<Texture, Project>::datas(); }

	Scene* scene() { return _scene; }
	const Scene* scene() const { return _scene; }
	// clang-format on

private:
	virtual void datasUpdated(const QList<Material*>& datas) override;
	virtual void datasUpdated(const QList<Texture*>& datas) override;

	QString _name{};
	QString _filePath{};
	bool _isModified{ false };
	QStringList _tags{};
	Scene* _scene{ nullptr };

signals:
	void nameUpdated();
	void filePathUpdated();
	void isModifiedUpdated();
	void tagsUpdated(const QStringList& tags);
	void materialsUpdated(const QList<Material*>& materials);
	void texturesUpdated(const QList<Texture*>& textures);
};
} // namespace eno
