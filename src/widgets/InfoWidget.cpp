#include "InfoWidget.hpp"

#include <QBoxLayout>
#include <QLabel>

#include "data/Scene.hpp"
#include "view/GraphicsView.hpp"
#include "widgets/common/SpinBox.hpp"

namespace eno {

InfoWidget::InfoWidget(QWidget* parent)
	: QWidget(parent) {}

void InfoWidget::init(Scene* scene, GraphicsView* graphicsView) {
	assert(scene);
	assert(graphicsView);

	_scene = scene;
	_graphicsView = graphicsView;
	_labelPosition = new QLabel(this);
	_labelPosition->setFixedWidth(60);

	_minXSpinBox = new SpinBox(this);
	_minYSpinBox = new SpinBox(this);
	_maxXSpinBox = new SpinBox(this);
	_maxYSpinBox = new SpinBox(this);

	_minXSpinBox->init(false);
	_minYSpinBox->init(false);
	_maxXSpinBox->init(true);
	_maxYSpinBox->init(true);

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

	connect(_scene, &Scene::rectUpdated, this, &InfoWidget::updateData);
	connect(_graphicsView, &GraphicsView::pointerPositionUpdated, this, &InfoWidget::updateData);

	connect(_minXSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), [this]() {
		_scene->setMin({ _minXSpinBox->value(), _scene->min().y() });
	});
	connect(_minYSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), [this]() {
		_scene->setMin({ _scene->min().x(), _minYSpinBox->value() });
	});
	connect(_maxXSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), [this]() {
		_scene->setMax({ _maxXSpinBox->value(), _scene->max().y() });
	});
	connect(_maxYSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), [this]() {
		_scene->setMax({ _scene->max().x(), _maxYSpinBox->value() });
	});
}

void InfoWidget::updateData() {
	const auto& pos = _graphicsView->pointerPosition();
	_labelPosition->setText(QString("%1, %2").arg(pos.x()).arg(pos.y()));

	const auto& min = _scene->min();
	const auto& max = _scene->max();
	_minXSpinBox->setValue(min.x());
	_minYSpinBox->setValue(min.y());
	_maxXSpinBox->setValue(max.x());
	_maxYSpinBox->setValue(max.y());
}
} // namespace eno
