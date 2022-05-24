#pragma once

#include <QVector3D>

static bool operator<(const QVector3D& p1, const QVector3D& p2) {
	if (p1.x() < p2.x())
		return true;
	if (p1.x() == p2.x()) {
		if (p1.y() < p2.y())
			return true;
		if (p1.y() == p2.y() && p1.z() < p2.z())
			return true;
	}
	return false;
}
