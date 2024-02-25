#include "RecentFiles.hpp"

#include "Preferences.hpp"

namespace eno {
RecentFiles::RecentFiles(QObject* parent)
	: QObject(parent) {}

void RecentFiles::reset() {
	_datas = Preferences::recentFiles();

	emit datasUpdated();
}

const QList<QString>& RecentFiles::datas() const {
	return _datas;
}

void RecentFiles::add(const QString& data) {
	if (_datas.contains(data)) {
		_datas.removeAll(data);
	}
	_datas.push_front(data);

	Preferences::setRecentFiles(_datas);

	emit datasUpdated();
}

void RecentFiles::remove(const QString& data) {
	if (_datas.contains(data)) {
		_datas.removeAll(data);

		Preferences::setRecentFiles(_datas);

		emit datasUpdated();
	}
}

} // namespace eno
