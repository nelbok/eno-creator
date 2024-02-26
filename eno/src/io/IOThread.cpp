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
		, _p{ parent } {}

	void init() {
		auto* parent = _p->_project->parent();
		// Not thread safe!!!
		if (_p->_type == Type::Load) {
			_p->_project->setParent(nullptr);
			_p->_project->moveToThread(this);
		}
		connect(this, &QThread::finished, QCoreApplication::instance(), [this, parent]() {
			if (_p->_type == Type::Load) {
				_p->_project->setParent(parent);
				emit _p->_project->nameUpdated();
				emit _p->_project->materialsUpdated();
				emit _p->_project->scene()->rectUpdated();
				emit _p->_project->scene()->objectsUpdated();
				_p->_project = nullptr;
			}
			disconnect(this, &QThread::finished, QCoreApplication::instance(), nullptr);
			emit _p->finished();
		});
	}

protected:
	virtual void run() override {
		assert(_p->_project);
		assert(_p->_filePath != "");
		switch (_p->_type) {
			case Type::Load:
				_p->load();
				// Not thread safe!!!
				_p->_project->moveToThread(QCoreApplication::instance()->thread());
				break;
			case Type::Save:
				_p->save();
				_p->_project = nullptr;
				break;
			default:
				assert(false);
				break;
		}
	}

private:
	IOThread* _p{ nullptr };
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
	_impl->init();
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
