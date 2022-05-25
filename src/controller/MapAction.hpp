#pragma once

#include <QObject>
#include <QColor>
#include <QVector2D>

class QMouseEvent;

namespace eno {
class Data;

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

	MapAction(Data* data, QObject* parent = nullptr);
	virtual ~MapAction() = default;

	void reset();

	void setTypeAction(TypeAction value);
	TypeAction typeAction() const;

	void setColor(const QColor& color);
	const QColor& color() const;

	void setDepth(float depth);
	float depth() const;

	void setPenWidth(int depth);
	int penWidth() const;

	void setZoom(Zoom zoom);
	Zoom zoom() const;

	const Data* data() const;

	void mousePressEvent(const QVector2D& pos);
	void mouseMoveEvent(const QVector2D& pos);

private:
	bool validPosition(const QVector3D& pos) const;
	void removeItem(const QVector2D& pos);
	void addItem(const QVector2D& pos);
	void pickColor(const QVector2D& pos);
	void resize(const QVector2D& pos);

	TypeAction _typeAction{};
	QColor _color{};
	float _depth{};
	int _penWidth{};
	Zoom _zoom{};
	Data* _data{ nullptr };
	QVector2D _currentPos{};

private:
	template<class Func>
	void changeItem(const QVector2D& pos, Func lambda) {
		const int min = 0 - _penWidth / 2;
		const int max = _penWidth - _penWidth / 2;
		for (int x = min; x < max; ++x) {
			for (int y = min; y < max; ++y) {
				const QVector3D vec{ pos.x() + x, pos.y() + y, _depth };
				if (validPosition(vec)) {
					lambda(vec);
				}
			}
		}
	}

signals:
	void colorUpdated();
	void depthUpdated();
	void penWidthUpdated();
	void zoomUpdated();
};
} // namespace eno