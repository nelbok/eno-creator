#include <eno/data/Container.hpp>
#include <eno/data/Project.hpp>

namespace eno {

Item::Item(Project* project)
	: QObject(project)
	, _project{ project }
	, _uuid{ QUuid::createUuid() } {}

Item::Item(const QUuid& uuid, Project* project)
	: QObject(project)
	, _project{ project }
	, _uuid{ uuid } {}

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
