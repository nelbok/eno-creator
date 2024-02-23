#pragma once

#include <QtCore/QObject>

namespace eno {
class BaseCommand;
class ListCommand;

class Commands : public QObject {
	Q_OBJECT

public:
	Commands(QObject* parent = nullptr);
	virtual ~Commands() = default;

	void add(BaseCommand* command);

	// clang-format off
	bool canUndo() const { return !_undoCommands.isEmpty(); }
	bool canRedo() const { return !_redoCommands.isEmpty(); }
	void undo();
	void redo();
	// clang-format on

	void beginList();
	void endList();

	void reset();

private:
	void internalAdd(BaseCommand* command);
	void clear(QList<BaseCommand*>& commands);

	QList<BaseCommand*> _undoCommands{};
	QList<BaseCommand*> _redoCommands{};
	QList<ListCommand*> _list{};

signals:
	void updated();
};
} // namespace eno
