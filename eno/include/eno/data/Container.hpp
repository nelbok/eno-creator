#pragma once

#include <QObject>
#include <QList>

namespace eno {
class Item : public QObject {
	Q_OBJECT
	Q_PROPERTY(int refCount READ refCount NOTIFY refCountUpdated)
	Q_PROPERTY(bool isAlive READ isAlive NOTIFY isAliveUpdated)

public:
	Item(QObject* parent = nullptr);
	virtual ~Item();

	// clang-format off
	int refCount() const { return _refCount; }
	void increaseRefCount();
	void decreaseRefCount();

	bool isAlive() const { return _isAlive; }
	void setIsAlive(bool isAlive);
	// clang-format on

private:
	int _refCount{ 0 };
	bool _isAlive{ true };

signals:
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
		datasUpdated();
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
			datasUpdated();
		}
	}

protected:
	void clear() {
		for (auto* data : _datas) {
			data->deleteLater();
		}
		_datas.clear();
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

	virtual void datasUpdated() = 0;

	TProject* _project{ nullptr };
	QList<TChild*> _datas{};
};
} // namespace eno
