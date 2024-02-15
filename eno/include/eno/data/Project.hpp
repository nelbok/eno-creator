#pragma once

#include <QtCore/QObject>

namespace eno {
class Materials;
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

	Materials* materials() { return _materials; }
	const Materials* materials() const { return _materials; }

	Scene* scene() { return _scene; }
	const Scene* scene() const { return _scene; }
	// clang-format on

private:
	QString _filePath{};
	bool _isModified{ false };

	Materials* _materials{ nullptr };
	Scene* _scene{ nullptr };

signals:
	void filePathUpdated();
	void isModifiedUpdated();
};
} // namespace eno
