#pragma once

#include <QVector3D>
#include <QColor>

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

static bool operator<(const QColor& p1, const QColor& p2) {
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
