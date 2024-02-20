#include "UndoRedo.hpp"

#include "command/BaseCommand.hpp"

namespace eno {
UndoRedo::UndoRedo(QObject* parent)
	: QObject(parent) {}

void UndoRedo::add(BaseCommand* command) {
	command->setParent(this);
	command->redo();
	_undoCommands.push_back(command);
	clear(_redoCommands);
	emit updated();
}

void UndoRedo::undo() {
	assert(!_undoCommands.isEmpty());
	auto* command = _undoCommands.takeLast();
	command->undo();
	_redoCommands.push_back(command);
	emit updated();
}

void UndoRedo::redo() {
	assert(!_redoCommands.isEmpty());
	auto* command = _redoCommands.takeLast();
	command->redo();
	_undoCommands.push_back(command);
	emit updated();
}

void UndoRedo::reset() {
	clear(_undoCommands);
	clear(_redoCommands);
	emit updated();
}

void UndoRedo::clear(QList<BaseCommand*>& commands) {
	for (auto* command : commands) {
		command->deleteLater();
	}
	commands.clear();
}

} // namespace eno
