#pragma once

#include "data/Material.hpp"
#include "data/Materials.hpp"
#include "data/Project.hpp"
#include "data/Scene.hpp"

#include <QTextStream>

namespace eno {

struct EnoConverter {
	static void convert1_0(QTextStream& buffer, Project* project) {
		assert(project);
		auto* scene = project->scene();

		// Retrieve min/max
		int minX{}, maxX{}, minY{}, maxY{};
		buffer >> minX >> maxX >> minY >> maxY;
		scene->setMin(QPoint{ minX, minY } / 10);
		scene->setMax(QPoint{ maxX, maxY } / 10);

		// Retrieve scene
		while (!buffer.atEnd()) {
			float x{}, y{}, z{};
			QString color{};
			buffer >> x >> y >> z >> color;

			// Fix a end of line bug, will be fixed again with materials
			if (color.isEmpty()) {
				continue;
			}

			Material* material = nullptr;
			for (auto* mat : *(project->materials())) {
				if (mat->name() == color) {
					material = mat;
					break;
				}
			}
			if (material == nullptr) {
				material = new Material(project);
				material->setName(color);
				material->setDiffuse(color);
				project->materials()->add(material);
			}

			scene->addItem(QVector3D{ x, y, z } / 10.f, material);

			int a = 0;
		}
	}
};
} // namespace eno
