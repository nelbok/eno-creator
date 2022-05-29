#include "InfoWidget.hpp"

#include <QBoxLayout>
#include <QLabel>
#include <QSpinBox>

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
	_labelPosition = new QLabel(this);
	_labelPosition->setFixedWidth(60);

	QHBoxLayout* layout = new QHBoxLayout(this);
	layout->addWidget(new QLabel("Position: ", this));
	layout->addWidget(_labelPosition);
	layout->addWidget(new QLabel("Scene: (", this));
	_minXSpinBox = initSpinBox(layout, false);
	layout->addWidget(new QLabel(", ", this));
	_minYSpinBox = initSpinBox(layout, false);
	layout->addWidget(new QLabel(") (", this));
	_maxXSpinBox = initSpinBox(layout, true);
	layout->addWidget(new QLabel(", ", this));
	_maxYSpinBox = initSpinBox(layout, true);
	layout->addWidget(new QLabel(")", this));

	connect(_data, &Data::rectUpdated, this, &InfoWidget::updateData);
	connect(_graphicsView, &GraphicsView::pointerPositionUpdated, this, &InfoWidget::updateData);
}

void InfoWidget::updateData() {
	const auto& pos = _graphicsView->pointerPosition();
	_labelPosition->setText(QString("%1, %2").arg(pos.x()).arg(pos.y()));

	const auto& min = _data->min();
	const auto& max = _data->max();
	_minXSpinBox->setValue(min.x());
	_minYSpinBox->setValue(min.y());
	_maxXSpinBox->setValue(max.x());
	_maxYSpinBox->setValue(max.y());
}

QSpinBox* InfoWidget::initSpinBox(QHBoxLayout* layout, bool positive) {
	auto* widget = new QSpinBox(this);
	widget->setFixedWidth(48);
	if (positive) {
		widget->setRange(5, 999);
		connect(widget, QOverload<int>::of(&QSpinBox::valueChanged), [this]() {
			_data->setMax({ static_cast<float>(_maxXSpinBox->value()), static_cast<float>(_maxYSpinBox->value()) });
		});
	} else {
		widget->setRange(-999, -5);
		connect(widget, QOverload<int>::of(&QSpinBox::valueChanged), [this]() {
			_data->setMin({ static_cast<float>(_minXSpinBox->value()), static_cast<float>(_minYSpinBox->value()) });
		});
	}
	layout->addWidget(widget);
	return widget;
}
} // namespace eno
