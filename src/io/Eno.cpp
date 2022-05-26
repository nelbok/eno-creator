#include "Eno.hpp"

#include <QFileInfo>
#include <QTextStream>

#include "data/Data.hpp"
#include "controller/MapAction.hpp"

namespace eno {
Eno::Eno(MapAction* mapAction)
	: _data(mapAction->_data) {}

bool Eno::save(const QString& path) {
	assert(_data);

	QFile file(path);
	if (!file.open(QIODevice::WriteOnly)) {
		return false;
	}

	QTextStream tampon(&file);
	tampon.setFieldWidth(8);

	// Write min/max
	const auto& min = _data->min() * 10.f;
	const auto& max = _data->max() * 10.f;
	tampon << min.x() << max.x() << min.y() << max.y() << Qt::endl;

	// Write scene
	for (const auto& item : *_data) {
		const auto& vec = item.first * 10.f;
		tampon << vec.x() << vec.y() << vec.z() << item.second.name() << Qt::endl;
	}

	file.close();

	_data->setFilePath(path);
	_data->setIsModified(false);

	return true;
}

bool Eno::load(const QString& path) {
	assert(_data);

	QFile file(path);
	if (!file.open(QIODevice::ReadOnly)) {
		assert(false);
		return false;
	}

	// Reset and begin transaction
	_data->blockSignals(true);
	_data->reset();
	_data->setFilePath(path);

	QTextStream buffer(&file);
	buffer.setFieldWidth(8);

	// Retrieve min/max
	float minX{}, maxX{}, minY{}, maxY{};
	buffer >> minX >> maxX >> minY >> maxY;
	_data->setMin(QVector2D{ minX, minY } / 10.f);
	_data->setMax(QVector2D{ maxX, maxY } / 10.f);

	// Retrieve scene
	while (!buffer.atEnd()) {
		float x{}, y{}, z{};
		QString color;
		buffer >> x >> y >> z >> color;

		_data->addItem(QVector3D{ x, y, z } / 10.f, { color });
	}

	file.close();

	_data->blockSignals(false);
	_data->setIsModified(false);
	_data->rectUpdated();
	_data->sceneUpdated();

	return true;
}
} // namespace eno
