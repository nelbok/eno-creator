#pragma once

#include <QtCore/QObject>

namespace eno {
class BaseCommand;

class UndoRedo : public QObject {
	Q_OBJECT

public:
	UndoRedo(QObject* parent = nullptr);
	virtual ~UndoRedo() = default;

	void add(BaseCommand* command);

	// clang-format off
	bool canUndo() const { return !_undoCommands.isEmpty(); }
	bool canRedo() const { return !_redoCommands.isEmpty(); }
	void undo();
	void redo();
	// clang-format on

	void reset();

private:
	void clear(QList<BaseCommand*>& commands);

	QList<BaseCommand*> _undoCommands{};
	QList<BaseCommand*> _redoCommands{};

signals:
	void updated();
};
} // namespace eno
