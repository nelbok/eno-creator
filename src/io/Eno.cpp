#include "Eno.hpp"

#include <QFileInfo>
#include <QTextStream>

#include "controller/MapAction.hpp"
#include "data/Material.hpp"
#include "data/Project.hpp"
#include "data/Scene.hpp"

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

	QTextStream buffer(&file);
	buffer.setFieldWidth(8);

	// Retrieve version
	QString bar{};
	auto version = 0u;
	buffer >> bar >> version;
	if (version != 1u) {
		assert(false);
		return false;
	}

	// Reset and begin transaction
	_project->blockSignals(true);
	_project->reset();
	_project->setFilePath(path);
	auto* scene = _project->scene();
	//When a project is resetted, a default materials is created
	const auto& oldMaterials = _project->materials();
	QList<Material*> newMaterials{};

	// Retrieve min/max
	int minX{}, maxX{}, minY{}, maxY{};
	buffer >> minX >> maxX >> minY >> maxY;
	scene->setMin(QPoint{ minX, minY } / 10);
	scene->setMax(QPoint{ maxX, maxY } / 10);

	// Retrieve scene
	while (!buffer.atEnd()) {
		float x{}, y{}, z{};
		QString color{};
		buffer >> x >> y >> z >> color;

		// Fix a end of line bug, will be fixed again with materials
		if (color.isEmpty()) {
			continue;
		}

		Material* material = nullptr;
		for (auto* mat : newMaterials) {
			if (mat->name() == color) {
				material = mat;
				break;
			}
		}
		if (material == nullptr) {
			material = new Material(_project);
			material->setName(color);
			material->setDiffuse(color);
		}
		newMaterials.append(material);

		scene->addItem(QVector3D{ x, y, z } / 10.f, material);
	}

	for (auto* mat : oldMaterials) {
		_project->removeMaterial(mat);
	}
	for (auto* mat : newMaterials) {
		_project->addMaterial(mat);
	}

	file.close();

	_project->blockSignals(false);
	_project->setIsModified(false);
	scene->rectUpdated();
	scene->sceneUpdated();

	return true;
}
} // namespace eno
