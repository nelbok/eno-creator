#pragma once

#include "BaseCommand.hpp"

namespace eno {
template<typename TClass, typename TContainer, typename TParent>
class CreateCommand : public BaseCommand {
public:
	CreateCommand(TContainer* container, TParent* parent, int count)
		: _container{ container }
		, _parent{ parent }
		, _count{ count } {
		assert(count > 0);
	}

	virtual ~CreateCommand() = default;

	virtual void undo() override {
		for (auto* instance : _instances) {
			assert(instance->isAlive());
			instance->setIsAlive(false);
		}
	}

	virtual void redo() override {
		if (_instances.isEmpty()) {
			for (int i = 0; i < _count; ++i) {
				_instances.append(new TClass(_parent));
			}
			_container->add(_instances);
		} else {
			for (auto* instance : _instances) {
				assert(!instance->isAlive());
				instance->setIsAlive(true);
			}
		}
	}

	virtual void clean() override {
		_container->remove(_instances);
		for (auto* instance : _instances)
			instance->deleteLater();
	}

	const QList<TClass*>& instances() const {
		return _instances;
	}

private:
	QList<TClass*> _instances{};
	TContainer* _container{ nullptr };
	TParent* _parent{ nullptr };
	int _count{ 1 };
};
} // namespace eno
