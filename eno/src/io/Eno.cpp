#include <eno/io/Eno.hpp>

#include <QtCore/QFileInfo>
#include <QtCore/QDataStream>
#include <QtCore/QSaveFile>

#include <eno/data/Material.hpp>
#include <eno/data/Object.hpp>
#include <eno/data/Project.hpp>
#include <eno/data/Scene.hpp>

namespace eno {
enum class EnoVersion : unsigned int {
	v1 = 1u,
};

void Eno::save() {
	assert(_project);

	QSaveFile file(_path);
	if (!file.open(QIODevice::WriteOnly)) {
		_result = Result::Error;
		return;
	}

	QDataStream stream(&file);
	stream.setVersion(QDataStream::Qt_5_15);

	// Write version
	stream << Eno::fileVersion;

	// Write materials
	const auto& materials = _project->materials();
	{
		int nbMaterials = materials.count();
		stream << nbMaterials;
		for (auto* material : materials) {
			if (isInterruptionRequested()) {
				break;
			}
			stream << material->_uuid << material->_name << material->_diffuse;
		}
	}

	auto* scene = _project->scene();
	{
		// Write min/max
		stream << scene->_min << scene->_max;

		// Write scene
		int nbItems = scene->objects().count();
		stream << nbItems;
		for (auto* object : scene->objects()) {
			if (isInterruptionRequested()) {
				break;
			}
			stream << object->position() << object->material()->_uuid;
		}
	}

	if (isInterruptionRequested()) {
		_result = Result::Canceled;
		return;
	}

	if (file.commit()) {
		_project->setFilePath(_path);
		_project->setIsModified(false);

		_result = Result::Success;
	} else {
		_result = Result::Error;
	}
}

void Eno::load() {
	assert(_project);

	QFile file(_path);
	if (!file.open(QIODevice::ReadOnly)) {
		_result = Result::Error;
		return;
	}

	// Reset and begin transaction
	_project->blockSignals(true);
	_project->reset();
	_project->setFilePath(_path);

	// Remove default material
	auto* defaultMaterial = *(_project->materials().begin());

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
	_project->remove(defaultMaterial);

	_result = Result::Success;

	if (isInterruptionRequested()) {
		_project->reset();
		_result = Result::Canceled;
		return;
	}

	_project->blockSignals(false);
	_project->setIsModified(false);
}

void Eno::loadV1(QDataStream& stream) {
	const auto mapMaterials = loadMaterialsV1(stream);
	loadSceneV1(stream, mapMaterials);
}

// Materials load functions
QMap<QUuid, Material*> Eno::loadMaterialsV1(QDataStream& stream) {
	QMap<QUuid, Material*> mapMaterials;
	{
		int nbMaterials = 0;
		stream >> nbMaterials;
		for (int i = 0; i < nbMaterials; ++i) {
			if (isInterruptionRequested()) {
				break;
			}
			auto* material = new Material(_project);
			stream >> material->_uuid >> material->_name >> material->_diffuse;
			_project->add(material);
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
			if (isInterruptionRequested()) {
				break;
			}

			QVector3D pos;
			QUuid uuid;
			stream >> pos >> uuid;

			auto* object = new Object(_project);
			object->setPosition(pos);
			object->setMaterial(mapMaterials.value(uuid));
			scene->add({ object });
		}
	}
}

} // namespace eno
