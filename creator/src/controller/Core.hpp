#pragma once

#include <QtCore/QObject>

namespace eno {
class Graphics;
class Material;
class Project;
class UndoRedo;

class Core : public QObject {
	Q_OBJECT

public:
	Core(Project* project, QObject* parent = nullptr);
	virtual ~Core() = default;

	void reset();

	Material* createMyMaterial();

	Graphics* graphics() const;
	Project* project() const;
	UndoRedo* undoRedo() const;

private:
	Graphics* _graphics{ nullptr };
	Project* _project{ nullptr };
	UndoRedo* _undoRedo{};
};
} // namespace eno
