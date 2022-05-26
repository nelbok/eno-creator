#pragma once

#include <QColor>
#include <QList>
#include <QMap>
#include <QVector3D>

class QString;

namespace eno {
class Data;
class MapAction;

class WavefrontOBJ {
public:
	struct Triangle {
		int one{};
		int two{};
		int three{};
	};

	static constexpr auto fileType = "Wavefront OBJ (*.obj)";

	WavefrontOBJ(MapAction* mapAction);
	virtual ~WavefrontOBJ() = default;

	bool save(const QString& path);

private:
	void compute();
	int getIndexBy(const QVector3D& vertex);
	void insertTriangle(const QColor& color, const Triangle& triangle);
	bool writeObjFile(const QString& path);
	bool writeMtlFile(const QString& path);

	Data* _data{ nullptr };
	QList<QVector3D> _vertices{};
	QMap<QColor, QList<Triangle>> _triangles;
};
} // namespace eno
