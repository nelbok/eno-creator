#pragma once

#include <QtCore/QObject>

namespace eno {

class BaseCommand : public QObject {
	Q_OBJECT

public:
	BaseCommand(QObject* parent = nullptr);
	virtual ~BaseCommand() = default;

	virtual void undo() = 0;
	virtual void redo() = 0;
	virtual void finalize() {}
	virtual void clean() {}
};
} // namespace eno
