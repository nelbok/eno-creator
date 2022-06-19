#pragma once

#include <QMap>
#include <QUuid>

#include "IOThread.hpp"

class QString;
class QDataStream;

namespace eno {
class Material;

class Eno : public IOThread {
public:
	static constexpr auto fileType = "ENO file (*.eno)";
	static constexpr auto fileVersion = 1u;

	using IOThread::IOThread;

protected:
	virtual bool save() override;
	virtual bool load() override;

private:
	void loadV1(QDataStream& stream);

	// Materials load functions
	QMap<QUuid, Material*> loadMaterialsV1(QDataStream& stream);

	// Scene load functions
	void loadSceneV1(QDataStream& stream, const QMap<QUuid, Material*>& mapMaterials);
};
} // namespace eno
