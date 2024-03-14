#include <eno/tools/Merger.hpp>

#include <eno/data/Object.hpp>
#include <eno/data/Scene.hpp>

namespace eno::Merger {
QList<Cuboid> fillData(const Scene* scene) {
	QList<Cuboid> data;

	const auto& objects = scene->objects();
	for (auto* object : objects) {
		Cuboid c;
		c.position = object->position();
		c.material = object->material();
		data.push_back(c);
	}

	return data;
}

QList<Cuboid> mergeData(const Scene* scene) {
	return mergeData(fillData(scene));
}

QList<Cuboid> mergeData(QList<Cuboid> data) {
	if (data.count() < 2)
		return data;

	for (auto& a : data) {
		int nbFacesFound = 0;
		for (auto& b : data) {
			// X
			if (a.position.x() + 1 == b.position.x() && a.position.y() == b.position.y() && a.position.z() == b.position.z()) {
				++nbFacesFound;
				if (a.material != b.material)
					continue;
				a.faces = a.faces ^ Geometry::PlaneNormal::PositiveX;
				b.faces = b.faces ^ Geometry::PlaneNormal::NegativeX;
			}
			// Y
			else if (a.position.x() == b.position.x() && a.position.y() + 1 == b.position.y() && a.position.z() == b.position.z()) {
				++nbFacesFound;
				if (a.material != b.material)
					continue;
				a.faces = a.faces ^ Geometry::PlaneNormal::PositiveY;
				b.faces = b.faces ^ Geometry::PlaneNormal::NegativeY;
			}
			// Z
			else if (a.position.x() == b.position.x() && a.position.y() == b.position.y() && a.position.z() + 1 == b.position.z()) {
				++nbFacesFound;
				if (a.material != b.material)
					continue;
				a.faces = a.faces ^ Geometry::PlaneNormal::PositiveZ;
				b.faces = b.faces ^ Geometry::PlaneNormal::NegativeZ;
			}
			if (nbFacesFound >= 3) {
				assert(nbFacesFound == 3);
				break;
			}
		}
	}

	return data;
}

} // namespace eno::Merger
