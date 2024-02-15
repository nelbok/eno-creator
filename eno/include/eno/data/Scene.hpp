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

class Material;
class Project;

using SceneData = QMap<QVector3D, Material*>;

class Scene : public QObject {
	friend class Eno;

	Q_OBJECT

public:
	Scene(Project* project);
	virtual ~Scene() = default;

	void reset();

	// clang-format off
	const QPoint& min() const { return _min; }
	const QPoint& max() const { return _max; }
	void setMin(const QPoint& min);
	void setMax(const QPoint& max);
	// clang-format on

	void addItem(const QVector3D& pos, Material* material);
	void removeItem(const QVector3D& pos);
	bool findItem(const QVector3D& pos) const;
	int countItems() const;
	Material* materialAt(const QVector3D& pos) const;

	SceneData::const_key_value_iterator begin() const;
	SceneData::const_key_value_iterator end() const;

private:
	void updateScene();

	Project* _project{ nullptr };

	QPoint _min{};
	QPoint _max{};

	SceneData _sceneData{};

signals:
	void rectUpdated();
	void dataUpdated();
};
} // namespace eno
