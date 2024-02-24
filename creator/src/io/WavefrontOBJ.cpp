#include "WavefrontOBJ.hpp"

#include <QtCore/QCoreApplication>
#include <QtCore/QFileInfo>

#include <eno/data/Material.hpp>
#include <eno/data/Project.hpp>
#include <eno/data/Scene.hpp>
#include <eno/tools/Utils.hpp>

#include "controller/Preferences.hpp"

namespace eno {
bool operator==(const WavefrontOBJ::Triangle& p1, const WavefrontOBJ::Triangle& p2) {
	return (p1.one == p2.one) && (p1.two == p2.two) && (p1.three == p2.three);
}

void WavefrontOBJ::save() {
	assert(_project);
	_result = Result::Success;
	_data = fillData(_project->scene());
	if (Preferences::generationOptimized()) {
		_data = mergeData(_data);
	}
	compute();
	writeObjFile();
	writeMtlFile();

	if (isInterruptionRequested()) {
		_result = Result::Canceled;
		return;
	}

	// We need to commit
	if (_result != Result::Error) {
		bool success = true;
		success &= _objFile.commit();
		success &= _mtlFile.commit();
		if (!success) {
			// We are doomed...
			_result = Result::Error;
		}
	}
}

void WavefrontOBJ::compute() {
	for (const auto& cube : _data) {
		if (isInterruptionRequested()) {
			return;
		}
		const auto& p = cube.position;
		const auto& s = cube.scale;
		auto v1 = getIndexBy(p + s * QVector3D(0.f, 0.f, 1.f));
		auto v2 = getIndexBy(p + s * QVector3D(1.f, 0.f, 1.f));
		auto v3 = getIndexBy(p + s * QVector3D(0.f, 1.f, 1.f));
		auto v4 = getIndexBy(p + s * QVector3D(1.f, 1.f, 1.f));
		auto v5 = getIndexBy(p + s * QVector3D(0.f, 1.f, 0.f));
		auto v6 = getIndexBy(p + s * QVector3D(1.f, 1.f, 0.f));
		auto v7 = getIndexBy(p + s * QVector3D(0.f, 0.f, 0.f));
		auto v8 = getIndexBy(p + s * QVector3D(1.f, 0.f, 0.f));

		auto* m = cube.material;
		insertTriangle(m, { v1, v2, v3 });
		insertTriangle(m, { v3, v2, v4 });
		insertTriangle(m, { v3, v4, v5 });
		insertTriangle(m, { v5, v4, v6 });
		insertTriangle(m, { v5, v6, v7 });
		insertTriangle(m, { v7, v6, v8 });
		insertTriangle(m, { v7, v8, v1 });
		insertTriangle(m, { v1, v8, v2 });
		insertTriangle(m, { v2, v8, v4 });
		insertTriangle(m, { v4, v8, v6 });
		insertTriangle(m, { v7, v1, v5 });
		insertTriangle(m, { v5, v1, v3 });
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

void WavefrontOBJ::writeCredentials(QTextStream& stream) {
	stream << "# " << qApp->applicationName() << " " << qApp->applicationVersion() << ": " << _project->projectName() << Qt::endl;
	stream << "# Created on " << QDateTime::currentDateTime().toString() << Qt::endl;
}

void WavefrontOBJ::writeObjFile() {
	_objFile.setFileName(_filePath);
	if (!_objFile.open(QIODevice::WriteOnly)) {
		_result = Result::Error;
		return;
	}

	QTextStream stream(&_objFile);
	writeCredentials(stream);
	stream << "o " << _project->projectName() << Qt::endl;

	// Material file name
	QFileInfo fileInfo(_filePath);
	stream << "mtllib " << fileInfo.baseName() << ".mtl" << Qt::endl;

	stream << "# vertices" << Qt::endl;
	for (const auto& vertex : _vertices) {
		if (isInterruptionRequested()) {
			return;
		}
		stream << "v " << vertex.x() << " " << vertex.y() << " " << vertex.z() << Qt::endl;
	}

	stream << "# triangles" << Qt::endl;
	for (auto it = _triangles.constKeyValueBegin(); it != _triangles.constKeyValueEnd(); ++it) {
		if (isInterruptionRequested()) {
			return;
		}
		Material* material = it->first;
		stream << "g " << material->name() << Qt::endl;
		stream << "usemtl " << material->name() << Qt::endl;
		for (const auto& triangle : it->second) {
			if (isInterruptionRequested()) {
				return;
			}
			// The index begin at 1 and not 0 like in the list
			stream << "f " << triangle.one + 1 << " " << triangle.two + 1 << " " << triangle.three + 1 << Qt::endl;
		}
	}
}

void WavefrontOBJ::writeMtlFile() {
	QFileInfo fileInfo(_filePath);
	QString pathMtl = fileInfo.path() + "/" + fileInfo.baseName() + ".mtl";

	_mtlFile.setFileName(pathMtl);
	if (!_mtlFile.open(QIODevice::WriteOnly)) {
		_result = Result::Error;
		return;
	}

	QTextStream stream(&_mtlFile);
	writeCredentials(stream);

	for (auto it = _triangles.constKeyValueBegin(); it != _triangles.constKeyValueEnd(); ++it) {
		if (isInterruptionRequested()) {
			return;
		}
		Material* material = it->first;
		const QColor& color = material->diffuse();
		stream << "newmtl " << material->name() << Qt::endl;
		stream << "Kd " << color.red() / 255.f << " " << color.green() / 255.f << " " << color.blue() / 255.f << Qt::endl;
		stream << Qt::endl;
	}
}
} // namespace eno
