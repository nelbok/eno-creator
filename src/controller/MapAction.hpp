#pragma once

#include <QObject>
#include <QVector2D>
#include <QVector3D>

class QMouseEvent;

namespace eno {
class Project;
class Material;

class MapAction : public QObject {
	Q_OBJECT

public:
	enum class TypeAction {
		Remove,
		Add,
		Picker,
		Resize,
	};
	Q_ENUM(TypeAction)

	enum class Zoom : int {
		x50 = 50,
		x100 = 100,
		x200 = 200,
		x400 = 400,
	};
	Q_ENUM(Zoom)
	static QString toString(Zoom zoom);

	static constexpr auto minDepth = -100;
	static constexpr auto maxDepth = 100;
	static constexpr auto minPenWidth = 1;
	static constexpr auto maxPenWidth = 50;

	MapAction(Project* project, QObject* parent = nullptr);
	virtual ~MapAction() = default;

	void reset();

	void setTypeAction(TypeAction value);
	TypeAction typeAction() const;

	void setMaterial(Material* material);
	Material* material() const;

	void setDepth(float depth);
	float depth() const;

	void setPenWidth(int depth);
	int penWidth() const;

	void setZoom(Zoom zoom);
	Zoom zoom() const;

	const Project* project() const;
	Project* project();

	void mousePressEvent(const QVector3D& pos);
	void mouseMoveEvent(const QVector3D& pos);
	const Qt::CursorShape cursorShape(const QVector3D& pos) const;

private:
	bool validPosition(const QVector3D& pos) const;
	void removeItem(const QVector3D& pos);
	void addItem(const QVector3D& pos);
	void pickColor(const QVector3D& pos);
	void resize(const QVector2D& pos);

	TypeAction _typeAction{};
	Material* _material{ nullptr };
	float _depth{};
	int _penWidth{};
	Zoom _zoom{};
	Project* _project{ nullptr };
	QVector2D _currentPos{};

private:
	template<class Func>
	void changeItem(const QVector3D& pos, Func lambda) {
		const int min = 0 - _penWidth / 2;
		const int max = _penWidth - _penWidth / 2;
		for (int x = min; x < max; ++x) {
			for (int y = min; y < max; ++y) {
				const QVector3D vec{ pos.x() + x, pos.y(), pos.z() + y };
				if (validPosition(vec)) {
					lambda(vec);
				}
			}
		}
	}

signals:
	void materialUpdated();
	void depthUpdated();
	void penWidthUpdated();
	void zoomUpdated();
};
} // namespace eno
