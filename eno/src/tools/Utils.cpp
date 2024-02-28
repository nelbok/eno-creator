#include <eno/tools/Utils.hpp>

#include <eno/data/Object.hpp>

namespace eno::Utils {
void boundingBox(const QVector3D& vec, QVector3D& min, QVector3D& max) {
	min.setX(std::min(min.x(), vec.x()));
	min.setY(std::min(min.y(), vec.y()));
	min.setZ(std::min(min.z(), vec.z()));
	max.setX(std::max(max.x(), vec.x() + 1));
	max.setY(std::max(max.y(), vec.y() + 1));
	max.setZ(std::max(max.z(), vec.z() + 1));
}

void boundingBox(const QList<QVector3D>& list, QVector3D& min, QVector3D& max) {
	assert(!list.isEmpty());
	for (const auto& vec : list) {
		boundingBox(vec, min, max);
	}
}

QPair<QVector3D, QVector3D> boundingBox(const QList<QVector3D>& list) {
	assert(!list.isEmpty());
	QVector3D min{};
	QVector3D max{};

	const auto& vec = list.first();
	min.setX(vec.x());
	min.setY(vec.y());
	min.setZ(vec.z());
	max.setX(vec.x() + 1);
	max.setY(vec.y() + 1);
	max.setZ(vec.z() + 1);

	boundingBox(list, min, max);

	return { min, max };
}

QPair<QVector3D, QVector3D> boundingBox(const QList<Object*>& list) {
	assert(!list.isEmpty());
	QVector3D min{};
	QVector3D max{};

	const auto& vec = list.first()->position();
	min.setX(vec.x());
	min.setY(vec.y());
	min.setZ(vec.z());
	max.setX(vec.x() + 1);
	max.setY(vec.y() + 1);
	max.setZ(vec.z() + 1);

	for (const auto& object : list) {
		boundingBox(object->position(), min, max);
	}

	return { min, max };
}

} // namespace eno::Utils

bool operator<(const QVector3D& p1, const QVector3D& p2) {
	// Y up
	if (p1.y() != p2.y())
		return p1.y() < p2.y();

	if (p1.z() != p2.z())
		return p1.z() < p2.z();
	return p1.x() < p2.x();
}

bool operator<(const QColor& p1, const QColor& p2) {
	if (p1.red() < p2.red())
		return true;
	if (p1.red() == p2.red()) {
		if (p1.green() < p2.green())
			return true;
		if (p1.green() == p2.green() && p1.blue() < p2.blue())
			return true;
	}
	return false;
}
