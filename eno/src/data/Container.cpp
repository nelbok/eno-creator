#include <eno/data/Container.hpp>

namespace eno {

Item::Item(QObject* parent)
	: QObject(parent) {}

Item::~Item() {
	assert(_refCount == 0);
}

void Item::increaseRefCount() {
	++_refCount;
	emit refCountUpdated();
}

void Item::decreaseRefCount() {
	assert(_refCount > 0);
	--_refCount;
	emit refCountUpdated();
}

void Item::setIsAlive(bool isAlive) {
	if (_isAlive != isAlive) {
		_isAlive = isAlive;
		emit isAliveUpdated();
	}
}
} // namespace eno
