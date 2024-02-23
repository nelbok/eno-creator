#pragma once

#include "BaseCommand.hpp"

namespace eno {
template<typename TClass, typename TContainer>
class DestroyCommand : public BaseCommand {
public:
	DestroyCommand(const QList<TClass*>& instances, TContainer* container)
		: _instances{ instances }
		, _container{ container } {
		assert(!instances.isEmpty());
	}

	virtual ~DestroyCommand() = default;

	virtual void undo() override {
		for (auto* instance : _instances) {
			assert(!instance->isAlive());
			instance->setIsAlive(true);
		}
	}

	virtual void redo() override {
		for (auto* instance : _instances) {
			assert(instance->isAlive());
			instance->setIsAlive(false);
		}
	}

	virtual void finalize() override {
		_container->remove(_instances);
		for (auto* instance : _instances)
			instance->deleteLater();
	}

private:
	QList<TClass*> _instances{};
	TContainer* _container{ nullptr };
};
} // namespace eno
