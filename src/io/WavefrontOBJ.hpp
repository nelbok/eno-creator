#pragma once

#include <QList>
#include <QMap>
#include <QVector3D>

#include "IOThread.hpp"

namespace eno {
class Material;

class WavefrontOBJ : public IOThread {
	Q_OBJECT

public:
	struct Triangle {
		int one{};
		int two{};
		int three{};
	};

	static constexpr auto fileType = "Wavefront OBJ (*.obj)";

	using IOThread::IOThread;

protected:
	virtual bool save() override;

private:
	void compute();
	int getIndexBy(const QVector3D& vertex);
	void insertTriangle(Material* material, const Triangle& triangle);
	bool writeObjFile();
	bool writeMtlFile();

	QList<QVector3D> _vertices{};
	QMap<Material*, QList<Triangle>> _triangles;
};
} // namespace eno
