#pragma once

#include <QtCore/QList>
#include <QtCore/QObject>
#include <QtCore/QUuid>

namespace eno {
class Project;

class Item : public QObject {
	Q_OBJECT
	Q_PROPERTY(QUuid uuid READ uuid NOTIFY uuidUpdated)
	Q_PROPERTY(int refCount READ refCount NOTIFY refCountUpdated)
	Q_PROPERTY(bool isAlive READ isAlive NOTIFY isAliveUpdated)

public:
	Item(Project* project);
	Item(const QUuid& uuid, Project* project);
	virtual ~Item();

	// clang-format off
	const QUuid& uuid() const { return _uuid; }

	int refCount() const { return _refCount; }
	void increaseRefCount();
	void decreaseRefCount();

	bool isAlive() const { return _isAlive; }
	void setIsAlive(bool isAlive);
	// clang-format on

protected:
	Project* _project{ nullptr };

private:
	QUuid _uuid{};
	int _refCount{ 0 };
	bool _isAlive{ true };

signals:
	void uuidUpdated();
	void refCountUpdated();
	void isAliveUpdated();
};

template<typename TChild, typename TProject>
class Container {
public:
	Container(TProject* project)
		: _project{ project } {
		static_assert(std::is_base_of<Item, TChild>(), "Child is not an Item");
	}

	void add(const QList<TChild*> datas) {
		for (auto* data : datas) {
			assert(data);
			assert(!_datas.contains(data));
			_datas.append(data);
		}
		_project->setIsModified(true);
		datasUpdated(_datas);
	}

	bool canRemove(const QList<TChild*> datas) {
		for (auto* data : datas) {
			assert(data);
			assert(_datas.contains(data));
			if (data->refCount() > 0) {
				return false;
			}
		}
		return true;
	}

	void remove(const QList<TChild*> datas) {
		if (canRemove(datas)) {
			for (auto* data : datas) {
				assert(data);
				assert(_datas.contains(data));
				_datas.removeAll(data);
			}
			_project->setIsModified(true);
			datasUpdated(_datas);
		}
	}

protected:
	void clear() {
		for (auto* data : _datas) {
			data->deleteLater();
		}
		_datas.clear();
		datasUpdated(_datas);
	}

	QList<TChild*> datas() const {
		QList<TChild*> datas;
		for (auto* data : _datas) {
			if (data->isAlive()) {
				datas.push_back(data);
			}
		}
		return datas;
	}

	virtual void datasUpdated(const QList<TChild*>& data) = 0;

	TProject* _project{ nullptr };
	QList<TChild*> _datas{};
};
} // namespace eno
