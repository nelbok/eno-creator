#pragma once

#include <QtCore/QMap>
#include <QtCore/QObject>
#include <QtCore/QPoint>
#include <QtGui/QVector3D>

#include <eno/data/Object.hpp>

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

class Project;

class Scene
	: public QObject
	, public Container<Object, Project> {
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
	QPoint fixMin(QPoint min) const;
	QPoint fixMax(QPoint max) const;

	using Container<Object, Project>::add;
	using Container<Object, Project>::canRemove;
	using Container<Object, Project>::remove;
	inline QList<Object*> objects() const { return Container::datas(); }
	// clang-format on

private:
	virtual void datasUpdated(const QList<Object*>& datas) override;

	QPoint _min{};
	QPoint _max{};

signals:
	void rectUpdated();
	void objectsUpdated(const QList<Object*>& objects);
};
} // namespace eno
