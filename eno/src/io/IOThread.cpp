#include <eno/io/IOThread.hpp>

#include <QtCore/QCoreApplication>
#include <QtCore/QThread>
#include <QtCore/QUrl>

#include <eno/data/Project.hpp>

namespace eno {

class IOThread::Impl : public QThread {
public:
	Impl(IOThread* parent)
		: QThread(parent)
		, _parent{ parent } {}

	void init(Project* project, Type type, const QString& filePath) {
		auto* parent = _parent->_project->parent();
		// Not thread safe!!!
		if (type == Type::Load) {
			_parent->_project->setParent(nullptr);
			_parent->_project->moveToThread(this);
		}
		connect(this, &QThread::finished, QCoreApplication::instance(), [this, type, parent]() {
			if (type == Type::Load) {
				_parent->_project->setParent(parent);
				emit _parent->_project->nameUpdated();
				emit _parent->_project->materialsUpdated();
				emit _parent->_project->scene()->rectUpdated();
				emit _parent->_project->scene()->objectsUpdated();
				_parent->_project = nullptr;
			}
			disconnect(this, &QThread::finished, QCoreApplication::instance(), nullptr);
			emit _parent->finished();
		});
	}

protected:
	virtual void run() override {
		assert(_parent->_project);
		assert(_parent->_filePath != "");
		switch (_parent->_type) {
			case Type::Load:
				_parent->load();
				// Not thread safe!!!
				_parent->_project->moveToThread(QCoreApplication::instance()->thread());
				break;
			case Type::Save:
				_parent->save();
				_parent->_project = nullptr;
				break;
			default:
				assert(false);
				break;
		}
	}

private:
	IOThread* _parent{ nullptr };
};

IOThread::IOThread(QObject* parent)
	: QObject(parent)
	, _impl{ new Impl(this) } {}

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
	_impl->init(project, type, filePath);
}

void IOThread::start() {
	_impl->start();
}

IOThread::Result IOThread::result() const {
	return _result;
}

void IOThread::requestInterruption() {
	_impl->requestInterruption();
}

bool IOThread::isInterruptionRequested() {
	return _impl->isInterruptionRequested();
}


void IOThread::save() {
	assert(false);
}

void IOThread::load() {
	assert(false);
}

} // namespace eno
