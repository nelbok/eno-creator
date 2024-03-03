#pragma once

#include <QtCore/QMap>
#include <QtCore/QUuid>

#include <eno/io/IOThread.hpp>

class QString;
class QDataStream;

namespace eno {
class Material;

// Eno Unit: meter
class Eno : public IOThread {
public:
	static constexpr auto fileType = "ENO file (*.eno)";
	static constexpr auto fileVersion = 4u;

	using IOThread::IOThread;

protected:
	virtual void save() override;
	virtual void load() override;
};
} // namespace eno
