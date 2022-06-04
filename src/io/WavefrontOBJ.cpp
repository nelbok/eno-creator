#include "WavefrontOBJ.hpp"

#include <QCoreApplication>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>

#include "data/Material.hpp"
#include "data/Project.hpp"
#include "data/Scene.hpp"

#include "Utils.hpp"

namespace eno {
bool operator==(const WavefrontOBJ::Triangle& p1, const WavefrontOBJ::Triangle& p2) {
	return (p1.one == p2.one) && (p1.two == p2.two) && (p1.three == p2.three);
}

WavefrontOBJ::WavefrontOBJ(const Project* project)
	: _project(project) {}

bool WavefrontOBJ::save(const QString& path) {
	assert(_project);
	bool success = true;

	compute();
	success &= writeObjFile(path);
	success &= writeMtlFile(path);

	return success;
}

void WavefrontOBJ::compute() {
	for (const auto& cube : *_project->scene()) {
		const auto& vec = cube.first;
		auto v1 = getIndexBy(vec + QVector3D(0.f, 0.f, 1.f));
		auto v2 = getIndexBy(vec + QVector3D(1.f, 0.f, 1.f));
		auto v3 = getIndexBy(vec + QVector3D(0.f, 1.f, 1.f));
		auto v4 = getIndexBy(vec + QVector3D(1.f, 1.f, 1.f));
		auto v5 = getIndexBy(vec + QVector3D(0.f, 1.f, 0.f));
		auto v6 = getIndexBy(vec + QVector3D(1.f, 1.f, 0.f));
		auto v7 = getIndexBy(vec + QVector3D(0.f, 0.f, 0.f));
		auto v8 = getIndexBy(vec + QVector3D(1.f, 0.f, 0.f));

		const auto& material = cube.second;
		insertTriangle(material, { v1, v2, v3 });
		insertTriangle(material, { v3, v2, v4 });
		insertTriangle(material, { v3, v4, v5 });
		insertTriangle(material, { v5, v4, v6 });
		insertTriangle(material, { v5, v6, v7 });
		insertTriangle(material, { v7, v6, v8 });
		insertTriangle(material, { v7, v8, v1 });
		insertTriangle(material, { v1, v8, v2 });
		insertTriangle(material, { v2, v8, v4 });
		insertTriangle(material, { v4, v8, v6 });
		insertTriangle(material, { v7, v1, v5 });
		insertTriangle(material, { v5, v1, v3 });
	}
}

int WavefrontOBJ::getIndexBy(const QVector3D& vertex) {
	auto index = _vertices.indexOf(vertex);
	if (index == -1) {
		index = _vertices.count();
		_vertices.append(vertex);
	}
	return index;
}

void WavefrontOBJ::insertTriangle(Material* material, const Triangle& triangle) {
	assert(material);
	auto& triangles = _triangles[material];
	if (!(triangles.contains(triangle))) {
		triangles.append(triangle);
	}
}

bool WavefrontOBJ::writeObjFile(const QString& path) {
	QFile file(path);
	if (!file.open(QIODevice::WriteOnly)) {
		return false;
	}

	QTextStream tampon(&file);
	tampon << "# " << qApp->applicationName() << " " << qApp->applicationVersion() << ": " << _project->projectName() << Qt::endl;
	tampon << "o " << _project->projectName() << Qt::endl;

	// Material file name
	QFileInfo fileInfo(path);
	tampon << "mtllib " << fileInfo.baseName() << ".mtl" << Qt::endl;

	tampon << "# vertices" << Qt::endl;
	for (const auto& vertex : _vertices) {
		tampon << "v " << vertex.x() << " " << vertex.y() << " " << vertex.z() << Qt::endl;
	}

	tampon << "# triangles" << Qt::endl;
	for (auto it = _triangles.constKeyValueBegin(); it != _triangles.constKeyValueEnd(); ++it) {
		Material* material = it->first;
		tampon << "g " << material->name() << Qt::endl;
		tampon << "usemtl " << material->name() << Qt::endl;
		for (const auto& triangle : it->second) {
			// The index begin at 1 and not 0 like in the list
			tampon << "f " << triangle.one + 1 << " " << triangle.two + 1 << " " << triangle.three + 1 << Qt::endl;
		}
	}

	file.close();

	return true;
}

bool WavefrontOBJ::writeMtlFile(const QString& path) {
	QFileInfo fileInfo(path);
	QString pathMtl = fileInfo.path() + "/" + fileInfo.baseName() + ".mtl";

	QFile file(pathMtl);
	if (!file.open(QIODevice::WriteOnly)) {
		return false;
	}

	QTextStream tampon(&file);
	for (auto it = _triangles.constKeyValueBegin(); it != _triangles.constKeyValueEnd(); ++it) {
		Material* material = it->first;
		const QColor& color = material->diffuse();
		tampon << "newmtl " << material->name() << Qt::endl;
		tampon << "Kd " << color.red() / 255.f << " " << color.green() / 255.f << " " << color.blue() / 255.f << Qt::endl;
		tampon << Qt::endl;
	}

	file.close();

	return true;
}
} // namespace eno
