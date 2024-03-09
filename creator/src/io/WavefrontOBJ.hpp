#pragma once

#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QSaveFile>
#include <QtGui/QVector3D>

#include <eno/tools/Merger.hpp>
#include <eno/io/IOThread.hpp>

namespace eno {
class Material;

class WavefrontOBJ : public IOThread {
	Q_OBJECT

public:
	struct Triangle {
		int v1{ -1 };
		int v2{ -1 };
		int v3{ -1 };

		int vt1{ -1 };
		int vt2{ -1 };
		int vt3{ -1 };

		int vn1{ -1 };
		int vn2{ -1 };
		int vn3{ -1 };
	};

	static constexpr auto fileType = "Wavefront OBJ (*.obj)";

	using IOThread::IOThread;

protected:
	virtual void save() override;

private:
	void compute();
	int getIndexForVertex(const QVector3D& vertex);
	int getIndexForUV(const QVector2D& uv);
	int getIndexForNormal(const QVector3D& normal);
	void insertTriangle(Material* material, const Triangle& triangle);
	void writeCredentials(QTextStream& stream);
	void writeObjFile();
	void writeMtlFile();
	void writeTextures();

	QList<Merger::Cuboid> _data{};
	QList<QVector3D> _vertices{};
	QList<QVector2D> _uvs{};
	QList<QVector3D> _normals{};
	QMap<Material*, QList<Triangle>> _triangles;
	QSaveFile _objFile;
	QSaveFile _mtlFile;
};
} // namespace eno
