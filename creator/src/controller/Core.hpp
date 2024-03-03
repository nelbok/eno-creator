#pragma once

#include <QtCore/QObject>

namespace eno {
class Commands;
class Graphics;
class Material;
class Project;
class RecentFiles;
class Texture;

class Core : public QObject {
	Q_OBJECT

public:
	Core(Project* project, QObject* parent = nullptr);
	virtual ~Core() = default;

	void reset();

	Material* createMyMaterial();
	Texture* createMyTexture();

	Commands* commands() const;
	Graphics* graphics() const;
	Project* project() const;
	RecentFiles* recentFiles() const;

private:
	Commands* _commands{ nullptr };
	Graphics* _graphics{ nullptr };
	Project* _project{ nullptr };
	RecentFiles* _recentFiles{ nullptr };
};
} // namespace eno
