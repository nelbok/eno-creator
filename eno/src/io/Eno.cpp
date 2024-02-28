#include <eno/io/Eno.hpp>

#include <QtCore/QFileInfo>
#include <QtCore/QDataStream>
#include <QtCore/QSaveFile>

#include <eno/data/Material.hpp>
#include <eno/data/Object.hpp>
#include <eno/data/Project.hpp>
#include <eno/data/Scene.hpp>

namespace eno {

void Eno::save() {
	assert(_project);

	QSaveFile file(_filePath);
	if (!file.open(QIODevice::WriteOnly)) {
		_result = Result::Error;
		return;
	}

	QDataStream stream(&file);
	stream.setVersion(QDataStream::Qt_5_15);

	// Write version
	stream << Eno::fileVersion;

	// Write project
	{ stream << _project->name(); }

	// Write materials
	{
		const auto& materials = _project->materials();
		int nbMaterials = materials.count();
		stream << nbMaterials;
		for (auto* material : materials) {
			if (isInterruptionRequested()) {
				break;
			}
			stream << material->uuid() << material->name() << material->diffuse();
		}
	}

	// Write scene
	{
		auto* scene = _project->scene();

		// Write min/max
		stream << scene->min() << scene->max();

		// Write scene
		const auto& objects = scene->objects();
		int nbObjects = objects.count();
		stream << nbObjects;
		for (auto* object : objects) {
			if (isInterruptionRequested()) {
				break;
			}
			stream << object->uuid() << object->position() << object->material()->uuid();
		}
	}

	if (isInterruptionRequested()) {
		_result = Result::Canceled;
		return;
	}

	if (file.commit()) {
		_project->setFilePath(_filePath);
		_project->setIsModified(false);

		_result = Result::Success;
	} else {
		_result = Result::Error;
	}
}

void Eno::load() {
	assert(_project);

	QFile file(_filePath);
	if (!file.open(QIODevice::ReadOnly)) {
		_result = Result::Error;
		return;
	}

	// Reset and begin transaction
	_project->blockSignals(true);
	_project->reset();
	_project->setFilePath(_filePath);

	QDataStream stream(&file);
	stream.setVersion(QDataStream::Qt_5_15);

	// Read version
	auto version = 0u;
	stream >> version;
	if (!(Eno::fileVersion >= version && version > 0u)) {
		_result = Result::Error;
		_project->reset();
		file.close();
		_project->blockSignals(false);
		_project->setIsModified(false);
		return;
	}

	try {
		// Read project
		if (version > 1) {
			QString name;
			stream >> name;
			_project->setName(name);
		} else {
			_project->setName("Unknown");
		}

		// Read materials
		QMap<QUuid, Material*> links;
		{
			int nb = 0;
			stream >> nb;
			for (int i = 0; i < nb; ++i) {
				if (isInterruptionRequested()) {
					break;
				}
				QUuid uuid;
				QString name;
				QColor diffuse;
				stream >> uuid >> name >> diffuse;

				// Create material
				auto* material = new Material(uuid, _project);
				material->setName(name);
				material->setDiffuse(diffuse);
				_project->add({ material });
				links.insert(uuid, material);
			}
		}

		// Read scene
		{
			auto* scene = _project->scene();
			// Write min/max
			QPoint min;
			QPoint max;
			stream >> min >> max;
			scene->setMin(min);
			scene->setMax(max);

			// Write scene
			int nb = 0;
			stream >> nb;
			for (int i = 0; i < nb; ++i) {
				if (isInterruptionRequested()) {
					break;
				}

				QUuid uuid;
				QVector3D pos;
				QUuid mUuid;
				if (version > 1) {
					stream >> uuid >> pos >> mUuid;
				} else {
					stream >> pos >> mUuid;
				}

				// Create object
				Object* object = nullptr;
				if (version > 1) {
					object = new Object(uuid, _project);
				} else {
					object = new Object(_project);
				}
				object->setPosition(pos);
				object->setMaterial(links.value(mUuid));
				scene->add({ object });
			}
		}
		_result = Result::Success;
	} catch (...) {
		_result = Result::Error;
		_project->reset();
	}
	file.close();

	if (isInterruptionRequested()) {
		_project->reset();
		_result = Result::Canceled;
	}

	_project->blockSignals(false);
	_project->setIsModified(false);
}
} // namespace eno
