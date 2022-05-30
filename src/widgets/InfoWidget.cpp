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

	_minXSpinBox = createSpinBox(false);
	_minYSpinBox = createSpinBox(false);
	_maxXSpinBox = createSpinBox(true);
	_maxYSpinBox = createSpinBox(true);

	QHBoxLayout* layout = new QHBoxLayout(this);
	layout->addWidget(new QLabel("Position: ", this));
	layout->addWidget(_labelPosition);
	layout->addWidget(new QLabel("Scene: (", this));
	layout->addWidget(_minXSpinBox);
	layout->addWidget(new QLabel(", ", this));
	layout->addWidget(_minYSpinBox);
	layout->addWidget(new QLabel(") (", this));
	layout->addWidget(_maxXSpinBox);
	layout->addWidget(new QLabel(", ", this));
	layout->addWidget(_maxYSpinBox);
	layout->addWidget(new QLabel(")", this));

	connect(_data, &Data::rectUpdated, this, &InfoWidget::updateData);
	connect(_graphicsView, &GraphicsView::pointerPositionUpdated, this, &InfoWidget::updateData);

	connect(_minXSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), [this]() {
		_data->setMin({ _minXSpinBox->value(), _data->min().y() });
	});
	connect(_minYSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), [this]() {
		_data->setMin({ _data->min().x(), _minYSpinBox->value() });
	});
	connect(_maxXSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), [this]() {
		_data->setMax({ _maxXSpinBox->value(), _data->max().y() });
	});
	connect(_maxYSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), [this]() {
		_data->setMax({ _data->max().x(), _maxYSpinBox->value() });
	});
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

QSpinBox* InfoWidget::createSpinBox(bool positive) {
	auto* widget = new QSpinBox(this);
	widget->setFixedWidth(48);
	if (positive) {
		widget->setRange(5, 999);
	} else {
		widget->setRange(-999, -5);
	}
	return widget;
}
} // namespace eno
