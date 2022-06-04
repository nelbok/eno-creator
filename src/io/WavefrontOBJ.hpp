#pragma once

#include <QList>
#include <QMap>
#include <QVector3D>

namespace eno {
class Project;
class Material;

class WavefrontOBJ {
public:
	struct Triangle {
		int one{};
		int two{};
		int three{};
	};

	static constexpr auto fileType = "Wavefront OBJ (*.obj)";

	WavefrontOBJ(const Project* project);
	virtual ~WavefrontOBJ() = default;

	bool save(const QString& path);

private:
	void compute();
	int getIndexBy(const QVector3D& vertex);
	void insertTriangle(Material* material, const Triangle& triangle);
	bool writeObjFile(const QString& path);
	bool writeMtlFile(const QString& path);

	const Project* _project{ nullptr };
	QList<QVector3D> _vertices{};
	QMap<Material*, QList<Triangle>> _triangles;
};
} // namespace eno
