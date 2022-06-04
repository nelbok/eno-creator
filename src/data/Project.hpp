#pragma once

#include <QObject>
#include <QList>

namespace eno {
class Material;
class Scene;

class Project : public QObject {
	Q_OBJECT

public:
	Project(QObject* parent = nullptr);
	virtual ~Project() = default;

	void init();
	void reset();

	QString projectName() const;

	// clang-format off
	const QString& filePath() const { return _filePath; }
	void setFilePath(const QString& path);

	bool isModified() const { return _isModified; }
	void setIsModified(bool isModifed);

	Scene* scene() { return _scene; }
	const Scene* scene() const { return _scene; }
	// clang-format on

	// FIXME: Make materials class to manage them
	const QList<Material*>& materials() const;
	void addMaterial(Material* material);
	void removeMaterial(Material* material);

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
