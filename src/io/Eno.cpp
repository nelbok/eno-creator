#include "Eno.hpp"

#include <QFileInfo>
#include <QDataStream>
#include <QTextStream>

#include "data/Materials.hpp"
#include "data/Material.hpp"
#include "data/Project.hpp"
#include "data/Scene.hpp"

namespace eno {
enum class EnoVersion : unsigned int {
	v1 = 1u,
};

bool Eno::save() {
	assert(_project);

	QFile file(_path);
	if (!file.open(QIODevice::WriteOnly)) {
		return false;
	}

	QDataStream stream(&file);
	stream.setVersion(QDataStream::Qt_5_15);

	// Write version
	stream << Eno::fileVersion;

	// Write materials
	auto* materials = _project->materials();
	{
		auto nbMaterials = materials->count();
		stream << nbMaterials;
		for (auto* material : *materials) {
			stream << material->_uuid << material->_name << material->_diffuse;
		}
	}

	auto* scene = _project->scene();
	{
		// Write min/max
		stream << scene->_min << scene->_max;

		// Write scene
		auto nbItems = scene->countItems();
		stream << nbItems;
		for (const auto& item : *scene) {
			stream << item.first << item.second->_uuid;
		}
	}
	file.close();

	_project->setFilePath(_path);
	_project->setIsModified(false);

	return true;
}

bool Eno::load() {
	assert(_project);

	QFile file(_path);
	if (!file.open(QIODevice::ReadOnly)) {
		assert(false);
		return false;
	}

	// Reset and begin transaction
	_project->blockSignals(true);
	auto* materials = _project->materials();
	materials->blockSignals(true);
	_project->reset();
	_project->setFilePath(_path);

	// Remove default material
	auto* defaultMaterial = *(materials->begin());

	QDataStream stream(&file);
	stream.setVersion(QDataStream::Qt_5_15);

	// Read version
	auto version = Eno::fileVersion;
	stream >> version;

	try {
		switch (static_cast<EnoVersion>(version)) {
			case EnoVersion::v1:
				loadV1(stream);
				break;
			default:
				assert(false);
		}
	} catch (...) {
		_project->reset();
	}
	file.close();

	// Remove old default material
	materials->remove(defaultMaterial);

	materials->blockSignals(false);
	_project->blockSignals(false);
	_project->setIsModified(false);

	return true;
}

void Eno::loadV1(QDataStream& stream) {
	const auto mapMaterials = loadMaterialsV1(stream);
	loadSceneV1(stream, mapMaterials);
}

	// Materials load functions
QMap<QUuid, Material*> Eno::loadMaterialsV1(QDataStream& stream) {
	QMap<QUuid, Material*> mapMaterials;
	auto* materials = _project->materials();
	{
		int nbMaterials = 0;
		stream >> nbMaterials;
		for (int i = 0; i < nbMaterials; ++i) {
			auto* material = materials->createMaterial();
			stream >> material->_uuid >> material->_name >> material->_diffuse;
			materials->add(material);
			mapMaterials.insert(material->_uuid, material);
		}
	}
	return mapMaterials;
}

// Scene load functions
void Eno::loadSceneV1(QDataStream& stream, const QMap<QUuid, Material*>& mapMaterials) {
	auto* scene = _project->scene();
	{
		// Write min/max
		stream >> scene->_min >> scene->_max;

		// Write scene
		int nbItems = 0;
		stream >> nbItems;
		for (int i = 0; i < nbItems; ++i) {
			QVector3D pos;
			QUuid uuid;
			Material* material = nullptr;

			stream >> pos >> uuid;
			material = mapMaterials.value(uuid);

			scene->addItem(pos, material);
		}
	}
}

} // namespace eno
