#include "Data.hpp"

#include "Utils.hpp"

namespace eno {
Data::Data(QObject* parent)
	: QObject(parent) {}

void Data::reset() {
	_min = { -5.f, -5.f };
	_max = { 5.f, 5.f };
	rectUpdated();

	_scene.clear();
	sceneUpdated();
}

void Data::setMin(const QVector2D& min) {
	_min = min;
	if (_min.x() > -5.f) {
		_min.setX(-5.f);
	}
	if (_min.y() > -5.f) {
		_min.setY(-5.f);
	}
	updateScene();
	rectUpdated();
}
void Data::setMax(const QVector2D& max) {
	_max = max;
	if (_max.x() < 5.f) {
		_max.setX(5.f);
	}
	if (_max.y() < 5.f) {
		_max.setY(5.f);
	}
	updateScene();
	rectUpdated();
}

void Data::addItem(const QVector3D& pos, const QColor& color) {
	_scene[pos] = color;
	sceneUpdated();
}

void Data::removeItem(const QVector3D& pos) {
	_scene.remove(pos);
	sceneUpdated();
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
		sceneUpdated();
	}
}
} // namespace eno
