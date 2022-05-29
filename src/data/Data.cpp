#include "Data.hpp"

#include <QFileInfo>

#include "Utils.hpp"

namespace eno {
Data::Data(QObject* parent)
	: QObject(parent) {}

void Data::reset() {
	setFilePath("");

	setMin({ -5, -5 });
	setMax({ 5, 5 });

	_scene.clear();
	sceneUpdated();

	setIsModified(false);
}

QString Data::projectName() const {
	if (!_filePath.isEmpty()) {
		const auto& fileInfo = QFileInfo(_filePath);
		if (fileInfo.exists()) {
			return fileInfo.baseName();
		}
	}
	return "New Project";
}

void Data::setFilePath(const QString& path) {
	if (_filePath != path) {
		_filePath = path;
		filePathUpdated();
	}
}

void Data::setIsModified(bool value) {
	if (_isModified != value) {
		_isModified = value;
		isModifiedUpdated();
	}
}

void Data::setMin(const QPoint& min) {
	if (_min != min) {
		_min = min;
		if (_min.x() > -5) {
			_min.setX(-5);
		}
		if (_min.y() > -5) {
			_min.setY(-5);
		}
		setIsModified(true);
		updateScene();
		rectUpdated();
	}
}
void Data::setMax(const QPoint& max) {
	if (_max != max) {
		_max = max;
		if (_max.x() < 5) {
			_max.setX(5);
		}
		if (_max.y() < 5) {
			_max.setY(5);
		}
		setIsModified(true);
		updateScene();
		rectUpdated();
	}
}

void Data::addItem(const QVector3D& pos, const QColor& color) {
	if (_scene[pos] != color) {
		_scene[pos] = color;
		setIsModified(true);
		sceneUpdated();
	}
}

void Data::removeItem(const QVector3D& pos) {
	if (_scene.contains(pos)) {
		_scene.remove(pos);
		setIsModified(true);
		sceneUpdated();
	}
}

bool Data::findItem(const QVector3D& pos) const {
	return _scene.contains(pos);
}

const QColor Data::colorAt(const QVector3D& pos) const {
	assert(findItem(pos));
	return _scene.value(pos, {});
}

MapItem::const_key_value_iterator Data::begin() const {
	return _scene.constKeyValueBegin();
}

MapItem::const_key_value_iterator Data::end() const {
	return _scene.constKeyValueEnd();
}

void Data::updateScene() {
	QList<QVector3D> itemsToDelete{};
	for (const auto& item : _scene.keys()) {
		if (item.x() < _min.x() || item.x() > _max.x() || item.y() < _min.y() || item.y() > _max.y()) {
			itemsToDelete.append(item);
		}
	}
	for (const auto& item : itemsToDelete) {
		_scene.remove(item);
	}
	if (!itemsToDelete.isEmpty()) {
		setIsModified(true);
		sceneUpdated();
	}
}
} // namespace eno
