#pragma once

#include <QMap>
#include <QUuid>

class QString;
class QDataStream;

namespace eno {
class MapAction;
class Material;
class Project;

class Eno {
public:
	static constexpr auto fileType = "ENO file (*.eno)";
	static constexpr auto fileVersion = 1u;

	Eno(MapAction* mapAction);
	virtual ~Eno() = default;

	bool save(const QString& path);
	bool load(const QString& path);

private:
	void loadV1(QDataStream& stream);

	// Materials load functions
	QMap<QUuid, Material*> loadMaterialsV1(QDataStream& stream);

	// Scene load functions
	void loadSceneV1(QDataStream& stream, const QMap<QUuid, Material*>& mapMaterials);

	Project* _project{ nullptr };
};
} // namespace eno
