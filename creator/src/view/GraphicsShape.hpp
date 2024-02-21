#pragma once

#include <QtWidgets/QGraphicsItem>

namespace eno {
class Core;

class GraphicsShape : public QGraphicsItem {
public:
	enum class Mode {
		Normal,
		Below,
	};

	GraphicsShape(Core* core, QGraphicsItem* parent = 0);

	void setMode(Mode mode);

	virtual QRectF boundingRect() const override;
	virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0) override;

private:
	Core* _core{ nullptr };
	Mode _mode{ Mode::Normal };
};
} // namespace eno
