#include <eno/tools/Merger.hpp>

#include <eno/data/Scene.hpp>

namespace eno {
QList<Cuboid> fillData(Scene* scene) {
	QList<Cuboid> data;

	for (const auto& cube : *scene) {
		Cuboid c;
		c.position = cube.first;
		c.material = cube.second;
		data.push_back(c);
	}

	return data;
}

QList<Cuboid> mergeData(Scene* scene) {
	return mergeData(fillData(scene));
}

QList<Cuboid> mergeData(QList<Cuboid> data) {
	if (data.count() < 2)
		return data;

	// X
	for (int ia = 0; ia < data.count() - 1; ++ia) {
		for (int ib = ia + 1; ib < data.count();) {
			auto& a = data[ia];
			const auto& b = data.at(ib);

			if (a.position.y() != b.position.y() || a.position.z() != b.position.z() || a.material != b.material) {
				++ib;
				continue;
			}

			if (a.position.x() + a.scale.x() == b.position.x() && a.scale.y() == b.scale.y() && a.scale.z() == b.scale.z()) {
				a.scale.setX(a.scale.x() + 1);
				data.removeAt(ib);
			} else {
				++ib;
			}
		}
	}

	// Y
	for (int ia = 0; ia < data.count() - 1; ++ia) {
		for (int ib = ia + 1; ib < data.count();) {
			auto& a = data[ia];
			const auto& b = data.at(ib);

			if (a.position.x() != b.position.x() || a.position.z() != b.position.z() || a.material != b.material) {
				++ib;
				continue;
			}

			if (a.scale.x() == b.scale.x() && a.position.y() + a.scale.y() == b.position.y() && a.scale.z() == b.scale.z()) {
				a.scale.setY(a.scale.y() + 1);
				data.removeAt(ib);
			} else {
				++ib;
			}
		}
	}

	// Z
	for (int ia = 0; ia < data.count() - 1; ++ia) {
		for (int ib = ia + 1; ib < data.count();) {
			auto& a = data[ia];
			const auto& b = data.at(ib);

			if (a.position.x() != b.position.x() || a.position.y() != b.position.y() || a.material != b.material) {
				++ib;
				continue;
			}

			if (a.scale.x() == b.scale.x() && a.scale.y() == b.scale.y() && a.position.z() + a.scale.z() == b.position.z()) {
				a.scale.setZ(a.scale.z() + 1);
				data.removeAt(ib);
			} else {
				++ib;
			}
		}
	}

	return data;
}

} // namespace eno
