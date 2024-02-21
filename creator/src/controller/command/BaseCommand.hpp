#pragma once

#include <QtCore/QObject>

namespace eno {

class BaseCommand : public QObject {
	Q_OBJECT;

public:
	virtual void undo() = 0;
	virtual void redo() = 0;
	virtual void finalize() {}
	virtual void clean() {}
};

template<typename Class, typename Container>
class CreateCommand : public BaseCommand {
public:
	CreateCommand(Container* parent)
		: _parent{ parent } {}

	virtual void undo() override {
		assert(_instance->isAlive());
		_instance->setIsAlive(false);
	}

	virtual void redo() override {
		if (_instance) {
			assert(!_instance->isAlive());
			_instance->setIsAlive(true);
		} else {
			_instance = new Class(_parent);
			_parent->add({ _instance });
		}
	}

	virtual void clean() override {
		_parent->remove({ _instance });
	}

	Class* instance() const {
		return _instance;
	}

private:
	Class* _instance{ nullptr };
	Container* _parent{ nullptr };
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
