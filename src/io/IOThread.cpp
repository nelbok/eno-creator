#include "IOThread.hpp"

#include <QCoreApplication>

#include "data/Project.hpp"

namespace eno {
IOThread::IOThread(Project* project, const QString& path, Type type)
	: _project(project)
	, _path{ path }
	, _type{ type } {
	// Not thread safe!!!
	if (type == Type::Load) {
		auto* parent = _project->parent();
		_project->setParent(nullptr);
		_project->moveToThread(this);
		connect(this, &IOThread::finished, QCoreApplication::instance(), [this, parent]() {
			_project->setParent(parent);
		});
	}
}

bool IOThread::result() const {
	return _result;
}

void IOThread::run() {
	switch (_type) {
		case Type::Load:
			_result = load();
			// Not thread safe!!!
			_project->moveToThread(QCoreApplication::instance()->thread());
			break;
		case Type::Save:
			_result = save();
			break;
		default:
			assert(false);
			break;
	}
}

bool IOThread::save() {
	assert(false);
	return false;
}

bool IOThread::load() {
	assert(false);
	return false;
}

} // namespace eno
