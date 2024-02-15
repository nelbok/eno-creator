#pragma once

#include <QtCore/QMap>
#include <QtCore/QUuid>

#include <eno/io/IOThread.hpp>

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
	virtual void save() override;
	virtual void load() override;

private:
	void loadV1(QDataStream& stream);

	// Materials load functions
	QMap<QUuid, Material*> loadMaterialsV1(QDataStream& stream);

	// Scene load functions
	void loadSceneV1(QDataStream& stream, const QMap<QUuid, Material*>& mapMaterials);
};
} // namespace eno
