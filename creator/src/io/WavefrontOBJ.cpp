#include "WavefrontOBJ.hpp"

#include <QtCore/QCoreApplication>
#include <QtCore/QFileInfo>

#include <eno/data/Material.hpp>
#include <eno/data/Project.hpp>
#include <eno/data/Scene.hpp>
#include <eno/data/Texture.hpp>
#include <eno/tools/Geometry.hpp>

#include "controller/Preferences.hpp"

namespace eno {
bool operator==(const WavefrontOBJ::Triangle& p1, const WavefrontOBJ::Triangle& p2) {
	return (p1.v1 == p2.v1) && (p1.v2 == p2.v2) && (p1.v3 == p2.v3) && (p1.vt1 == p2.vt1) && (p1.vt2 == p2.vt2) && (p1.vt3 == p2.vt3) && (p1.vn1 == p2.vn1) && (p1.vn2 == p2.vn2) && (p1.vn3 == p2.vn3);
}

void WavefrontOBJ::save() {
	assert(_project);
	_result = Result::Success;
	_data = Merger::fillData(_project->scene());
	if (Preferences::generationOptimized()) {
		_data = mergeData(_data);
	}
	compute();
	writeObjFile();
	writeMtlFile();
	writeTextures();

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
	quint32 baseIndex = 0;
	QList<int> vIdx, tIdx, nIdx;
	for (const auto& cube : _data) {
		if (isInterruptionRequested()) {
			return;
		}
		auto* m = cube.material;

		{
			const auto& nbElements = Geometry::stride() / sizeof(float);
			const auto& vertexData = Geometry::createCuboidVertexData(cube.position, cube.faces);
			const float* d = reinterpret_cast<const float*>(vertexData.data());
			const qsizetype iMax = vertexData.length() / sizeof(float);
			for (qsizetype i = 0; i < iMax;) {
				vIdx.append(getIndexForVertex({ d[i + 0], d[i + 1], d[i + 2] }));
				tIdx.append(getIndexForUV({ d[i + 3], d[i + 4] }));
				nIdx.append(getIndexForNormal({ d[i + 5], d[i + 6], d[i + 7] }));

				i += nbElements;
			}
		}

		{
			const auto& indexData = Geometry::createCuboidIndexData(baseIndex, cube.faces);
			const quint32* d = reinterpret_cast<const quint32*>(indexData.data());
			const qsizetype iMax = indexData.length() / sizeof(quint32);
			for (qsizetype i = 0; i < iMax;) {
				insertTriangle(m, {
														vIdx.at(d[i + 0]),
														vIdx.at(d[i + 1]),
														vIdx.at(d[i + 2]),
														tIdx.at(d[i + 0]),
														tIdx.at(d[i + 1]),
														tIdx.at(d[i + 2]),
														nIdx.at(d[i + 0]),
														nIdx.at(d[i + 1]),
														nIdx.at(d[i + 2]),
													});

				i += 3;
			}
		}
	}
}

int WavefrontOBJ::getIndexForVertex(const QVector3D& vertex) {
	auto index = _vertices.indexOf(vertex);
	if (index == -1) {
		index = _vertices.count();
		_vertices.append(vertex);
	}
	return index;
}

int WavefrontOBJ::getIndexForUV(const QVector2D& uv) {
	auto index = _uvs.indexOf(uv);
	if (index == -1) {
		index = _uvs.count();
		_uvs.append(uv);
	}
	return index;
}

int WavefrontOBJ::getIndexForNormal(const QVector3D& normal) {
	auto index = _normals.indexOf(normal);
	if (index == -1) {
		index = _normals.count();
		_normals.append(normal);
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
	stream << "# " << qApp->applicationName() << " " << qApp->applicationVersion() << ": " << _project->name() << Qt::endl;
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
	stream << "o " << _project->name() << Qt::endl;

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

	stream << Qt::endl;

	stream << "# uvs" << Qt::endl;
	for (const auto& uv : _uvs) {
		if (isInterruptionRequested()) {
			return;
		}
		stream << "vt " << uv.x() << " " << uv.y() << Qt::endl;
	}

	stream << Qt::endl;

	stream << "# normals" << Qt::endl;
	for (const auto& normal : _normals) {
		if (isInterruptionRequested()) {
			return;
		}
		stream << "vn " << normal.x() << " " << normal.y() << " " << normal.z() << Qt::endl;
	}

	stream << Qt::endl;

	stream << "# triangles" << Qt::endl;
	for (auto it = _triangles.constKeyValueBegin(); it != _triangles.constKeyValueEnd(); ++it) {
		if (isInterruptionRequested()) {
			return;
		}
		Material* material = it->first;
		stream << "g " << material->name() << Qt::endl;
		stream << "usemtl " << material->name() << Qt::endl;
		for (const auto& t : it->second) {
			if (isInterruptionRequested()) {
				return;
			}
			// The index begin at 1 and not 0 like in the list
			stream << "f";
			stream << " " << t.v1 + 1 << "/" << t.vt1 + 1 << "/" << t.vn1 + 1;
			stream << " " << t.v2 + 1 << "/" << t.vt2 + 1 << "/" << t.vn2 + 1;
			stream << " " << t.v3 + 1 << "/" << t.vt3 + 1 << "/" << t.vn3 + 1;
			stream << Qt::endl;
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
		const auto& color = material->diffuse();
		auto* dTexture = material->diffuseMap();
		float opacity = material->opacity();
		auto* oTexture = material->opacityMap();
		stream << "newmtl " << material->name() << Qt::endl;
		stream << "Kd " << color.red() / 255.f << " " << color.green() / 255.f << " " << color.blue() / 255.f << Qt::endl;
		if (dTexture) {
			stream << "map_Kd " + dTexture->name() + ".png" << Qt::endl;
		}
		stream << "d " << opacity << Qt::endl;
		if (oTexture) {
			stream << "map_d " + oTexture->name() + ".png" << Qt::endl;
		}
		stream << Qt::endl;
	}
}

void WavefrontOBJ::writeTextures() {
	QFileInfo fileInfo(_filePath);

	const auto& textures = _project->textures();
	for (auto* texture : textures) {
		if (isInterruptionRequested()) {
			return;
		}
		// Is texture used?
		if (texture->refCount() == 0) {
			return;
		}

		texture->image().save(fileInfo.path() + "/" + texture->name() + ".png");
	}
}

} // namespace eno
