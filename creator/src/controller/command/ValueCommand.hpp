#pragma once

#include "BaseCommand.hpp"

namespace eno {
class Commands;

template<typename TClass, typename TFunc, typename TType>
class ValueCommand : public BaseCommand {
public:
	ValueCommand(TClass* instance, TFunc setter, TType oldValue, TType newValue)
		: _instance{ instance }
		, _setter{ setter }
		, _oldValue{ oldValue }
		, _newValue{ newValue } {
		assert(oldValue != newValue);
	}

	virtual ~ValueCommand() = default;

	virtual void undo() override {
		(_instance->*_setter)(_oldValue);
	}

	virtual void redo() override {
		(_instance->*_setter)(_newValue);
	}

private:
	TClass* _instance{ nullptr };
	TFunc _setter{};
	TType _oldValue{};
	TType _newValue{};
};

template<typename TClass, typename TSetter, typename TGetter, typename TType>
void addValueCommand(Commands* commands, TClass* instance, TSetter setter, TGetter getter, TType newValue) {
	const TType& oldValue = (instance->*getter)();
	if (oldValue != newValue) {
		commands->add(new ValueCommand(instance, setter, oldValue, newValue));
	}
}
} // namespace eno
