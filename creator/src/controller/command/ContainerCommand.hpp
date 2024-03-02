#pragma once

#include "BaseCommand.hpp"

namespace eno {
class Commands;

template<typename TClass, typename TFunc1, typename TFunc2, typename TType>
class ContainerCommand : public BaseCommand {
public:
	ContainerCommand(TClass* instance, TFunc1 redo, TFunc2 undo, const QList<TType>& values)
		: _instance{ instance }
		, _redo{ redo }
		, _undo{ undo }
		, _values{ values } {}

	virtual ~ContainerCommand() = default;

	virtual void undo() override {
		(_instance->*_undo)(_values);
	}

	virtual void redo() override {
		(_instance->*_redo)(_values);
	}

private:
	TClass* _instance{ nullptr };
	TFunc1 _redo{};
	TFunc2 _undo{};
	QList<TType> _values;
};
} // namespace eno
