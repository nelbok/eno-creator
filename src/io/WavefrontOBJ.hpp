#pragma once

#include <QList>
#include <QMap>
#include <QVector3D>
#include <QSaveFile>

#include "data/Merger.hpp"
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
	virtual void save() override;

private:
	void compute();
	int getIndexBy(const QVector3D& vertex);
	void insertTriangle(Material* material, const Triangle& triangle);
	void writeCredentials(QTextStream& stream);
	void writeObjFile();
	void writeMtlFile();

	QList<Cuboid> _data{};
	QList<QVector3D> _vertices{};
	QMap<Material*, QList<Triangle>> _triangles;
	QSaveFile _objFile;
	QSaveFile _mtlFile;
};
} // namespace eno
