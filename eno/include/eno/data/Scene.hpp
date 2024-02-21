#pragma once

#include <QtCore/QMap>
#include <QtCore/QObject>
#include <QtCore/QPoint>
#include <QtGui/QVector3D>

namespace eno {
//  Data Graphical Representation
//
//  Y
//  |  Z
//  | /       = SceneItem
//  |/
//  +-----X
// 0
//
//  Min / Max
//     +-------+ max (X,Z)
//    /       /
//   /       /
//  +-------+
// 0 = min (X,Z)

class Object;
class Project;

class Scene : public QObject {
	friend class Eno;

	Q_OBJECT
	Q_PROPERTY(QPoint min READ min WRITE setMin NOTIFY rectUpdated)
	Q_PROPERTY(QPoint max READ max WRITE setMax NOTIFY rectUpdated)
	Q_PROPERTY(QList<Object*> objects READ objects NOTIFY objectsUpdated)

public:
	static constexpr QPoint defaultSceneMin = { -5, -5 };
	static constexpr QPoint defaultSceneMax = { 5, 5 };

	Scene(Project* project);
	virtual ~Scene();

	void reset();
	void clear();

	// clang-format off
	const QPoint& min() const { return _min; }
	const QPoint& max() const { return _max; }
	void setMin(const QPoint& min);
	void setMax(const QPoint& max);

	void add(const QList<Object*>& objects);
	void remove(const QList<Object*>& objects);
	QList<Object*> objects() const { return _objects; }
	// clang-format on

private:
	void updateScene();

	Project* _project{ nullptr };

	QPoint _min{};
	QPoint _max{};

	QList<Object*> _objects{};

signals:
	void rectUpdated();
	void objectsUpdated();
};
} // namespace eno
