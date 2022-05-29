#include "InfoWidget.hpp"

#include <QBoxLayout>
#include <QLabel>

#include "data/Data.hpp"
#include "view/GraphicsView.hpp"

namespace eno {

InfoWidget::InfoWidget(QWidget* parent)
	: QWidget(parent) {}

void InfoWidget::init(Data* data, GraphicsView* graphicsView) {
	assert(data);
	assert(graphicsView);

	_data = data;
	_graphicsView = graphicsView;
	_label = new QLabel(this);

	QHBoxLayout* layout = new QHBoxLayout(this);
	layout->addWidget(_label);

	connect(_data, &Data::rectUpdated, this, &InfoWidget::updateData);
	connect(_graphicsView, &GraphicsView::pointerPositionUpdated, this, &InfoWidget::updateData);
}

void InfoWidget::updateData() {
	const auto& pos = _graphicsView->pointerPosition();
	const auto& min = _data->min();
	const auto& max = _data->max();
	_label->setText(QString("Position: %1, %2 Scene: (%3, %4) (%5, %6)").arg(pos.x()).arg(pos.y()).arg(min.x()).arg(min.y()).arg(max.x()).arg(max.y()));
}
} // namespace eno
