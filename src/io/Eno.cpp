#include "Eno.hpp"

#include <QFileInfo>
#include <QTextStream>

#include "controller/MapAction.hpp"
#include "data/Materials.hpp"
#include "data/Project.hpp"
#include "data/Scene.hpp"

#include "EnoConverter.hpp"

namespace eno {
Eno::Eno(MapAction* mapAction)
	: _project(mapAction->_project) {}

bool Eno::save(const QString& path) {
	assert(_project);

	/*	QFile file(path);
	if (!file.open(QIODevice::WriteOnly)) {
		return false;
	}

	QTextStream tampon(&file);
	tampon.setFieldWidth(8);

	// Write version
	tampon << "Version:" << Eno::fileVersion << Qt::endl;

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
	_data->setIsModified(false);*/

	return true;
}

bool Eno::load(const QString& path) {
	assert(_project);

	QFile file(path);
	if (!file.open(QIODevice::ReadOnly)) {
		assert(false);
		return false;
	}

	// Reset and begin transaction
	_project->blockSignals(true);
	_project->reset();
	_project->setFilePath(path);

	// Remove default material
	auto* materials = _project->materials();
	auto* defaultMaterial = *(materials->begin());

	QTextStream buffer(&file);
	buffer.setFieldWidth(8);

	// Retrieve version
	QString bar{};
	auto version = 1.f;
	buffer >> bar >> version;

	try {
		if (version == 1.0f) {
			EnoConverter::convert1_0(buffer, _project);
		} else {
			assert(false);
		}
	} catch (...) {
		_project->reset();
	}
	file.close();

	// Remove old default material
	materials->remove(defaultMaterial);

	_project->blockSignals(false);
	_project->setIsModified(false);
	_project->materials()->updated();
	_project->scene()->rectUpdated();
	_project->scene()->dataUpdated();

	return true;
}
} // namespace eno
