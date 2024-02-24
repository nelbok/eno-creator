#include <eno/io/IOThread.hpp>

#include <QtCore/QCoreApplication>
#include <QtCore/QUrl>

#include <eno/data/Project.hpp>

namespace eno {
IOThread::IOThread(QObject* parent)
	: QThread(parent) {}

void IOThread::init(Project* project, Type type, const QUrl& url) {
	assert(url.isValid());
	if (url.isLocalFile())
		init(project, type, url.toLocalFile());
	else
		init(project, type, url.toString());
}

void IOThread::init(Project* project, Type type, const QString& filePath) {
	_project = project;
	_type = type;
	_filePath = filePath;
	// Not thread safe!!!
	if (type == Type::Load) {
		auto* parent = _project->parent();
		_project->setParent(nullptr);
		_project->moveToThread(this);
		connect(this, &IOThread::finished, QCoreApplication::instance(), [this, parent]() {
			_project->setParent(parent);
			emit _project->materialsUpdated();
			emit _project->scene()->rectUpdated();
			emit _project->scene()->objectsUpdated();
			_project = nullptr;
			disconnect(this, &IOThread::finished, QCoreApplication::instance(), nullptr);
		});
	}
}

IOThread::Result IOThread::result() const {
	return _result;
}

void IOThread::run() {
	assert(_project);
	assert(_filePath != "");
	switch (_type) {
		case Type::Load:
			load();
			// Not thread safe!!!
			_project->moveToThread(QCoreApplication::instance()->thread());
			break;
		case Type::Save:
			save();
			_project = nullptr;
			break;
		default:
			assert(false);
			break;
	}
}

void IOThread::save() {
	assert(false);
}

void IOThread::load() {
	assert(false);
}

} // namespace eno
