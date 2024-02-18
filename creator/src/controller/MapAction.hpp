#pragma once

#include <QtCore/QObject>
#include <QtGui/QVector2D>
#include <QtGui/QVector3D>

#include <eno/data/Preferences.hpp>

class QMouseEvent;

namespace eno {
class Project;
class Material;

class MapAction : public QObject {
	Q_OBJECT

public:
	MapAction(Project* project, QObject* parent = nullptr);
	virtual ~MapAction() = default;

	void reset();

	void setTypeAction(Preferences::TypeAction value);
	Preferences::TypeAction typeAction() const;

	void setMaterial(Material* material);
	Material* material() const;

	void setDepth(int depth);
	int depth() const;

	void setPenWidth(int depth);
	int penWidth() const;

	void setZoom(Preferences::Zoom zoom);
	Preferences::Zoom zoom() const;

	const Project* project() const;
	Project* project();

	void mousePressEvent(const QVector3D& pos);
	void mouseMoveEvent(const QVector3D& pos);
	Qt::CursorShape cursorShape() const;

private:
	bool validPosition(const QVector3D& pos) const;
	void removeItem(const QVector3D& pos);
	void addItem(const QVector3D& pos);
	void pickColor(const QVector3D& pos);
	void resize(const QVector2D& pos);

	Preferences::TypeAction _typeAction{};
	Material* _material{ nullptr };
	int _depth{};
	int _penWidth{};
	Preferences::Zoom _zoom{};
	Project* _project{ nullptr };
	QVector2D _currentPos{};

private:
	template<class Func>
	void changeItem(const QVector3D& pos, Func lambda) {
		const int min = 0 - _penWidth / 2;
		const int max = _penWidth - _penWidth / 2;
		QList<QVector3D> vecs;
		for (int x = min; x < max; ++x) {
			for (int y = min; y < max; ++y) {
				const QVector3D vec{ pos.x() + x, pos.y(), pos.z() + y };
				if (validPosition(vec)) {
					vecs.append(vec);
				}
			}
		}
		if (!vecs.isEmpty()) {
			lambda(vecs);
		}
	}

signals:
	void materialUpdated();
	void depthUpdated();
	void penWidthUpdated();
	void zoomUpdated();
};
} // namespace eno
