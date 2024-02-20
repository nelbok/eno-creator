#pragma once

#include <QtCore/QObject>

namespace eno {

class BaseCommand : public QObject {
	Q_OBJECT;

public:
	virtual void undo() = 0;
	virtual void redo() = 0;
};

template<typename Class, typename Func, typename Type>
class ValueCommand : public BaseCommand {
public:
	ValueCommand(Class* instance, Func member, Type oldValue, Type newValue)
		: _instance{ instance }
		, _member(member)
		, _oldValue{ oldValue }
		, _newValue{ newValue } {}

	virtual void undo() override {
		(_instance->*_member)(_oldValue);
	}

	virtual void redo() override {
		(_instance->*_member)(_newValue);
	}

private:
	Class* _instance{ nullptr };
	Func _member{};
	Type _oldValue{};
	Type _newValue{};
};
} // namespace eno
