#pragma once

#include <QtWidgets/QGraphicsItem>

namespace eno {
class MapAction;

class GraphicsShape : public QGraphicsItem {
public:
	enum class Mode {
		Normal,
		Below,
	};

	GraphicsShape(MapAction* mapAction, QGraphicsItem* parent = 0);

	void setMode(Mode mode);

	virtual QRectF boundingRect() const override;
	virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0);

private:
	MapAction* _mapAction{nullptr};
	Mode _mode{ Mode::Normal };
};
} // namespace srp_creator
