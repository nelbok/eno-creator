#pragma once

#include <QtCore/QList>
#include <QtCore/QPair>
#include <QtGui/QColor>
#include <QtGui/QVector3D>

namespace eno {
class Object;
namespace Utils {
void boundingBox(const QVector3D& vec, QVector3D& min, QVector3D& max);
void boundingBox(const QList<QVector3D>& list, QVector3D& min, QVector3D& max);
QPair<QVector3D, QVector3D> boundingBox(const QList<QVector3D>& list);
QPair<QVector3D, QVector3D> boundingBox(const QList<Object*>& list);
} // namespace Utils
} // namespace eno

bool operator<(const QVector3D& p1, const QVector3D& p2);
bool operator<(const QColor& p1, const QColor& p2);
