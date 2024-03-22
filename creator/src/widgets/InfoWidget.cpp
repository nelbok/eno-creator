#include "InfoWidget.hpp"

#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QLabel>

#include <eno/data/Scene.hpp>

#include "controller/command/SceneCommand.hpp"
#include "view/GraphicsView.hpp"
#include "widgets/common/BoundingBox.hpp"

namespace eno {

InfoWidget::InfoWidget(QWidget* parent)
	: QWidget(parent) {}

void InfoWidget::init(Commands* commands, Scene* scene, GraphicsView* graphicsView) {
	assert(commands);
	assert(scene);
	assert(graphicsView);

	_scene = scene;
	_graphicsView = graphicsView;
	_labelPosition = new QLabel(this);
	_labelPosition->setFixedWidth(60);

	_minX = new BoundingBox(this);
	_minY = new BoundingBox(this);
	_maxX = new BoundingBox(this);
	_maxY = new BoundingBox(this);

	_minX->init(false);
	_minY->init(false);
	_maxX->init(true);
	_maxY->init(true);

	QHBoxLayout* layout = new QHBoxLayout(this);
	layout->addWidget(new QLabel("Position: ", this));
	layout->addWidget(_labelPosition);
	layout->addWidget(new QLabel("Scene: (", this));
	layout->addWidget(_minX);
	layout->addWidget(new QLabel(", ", this));
	layout->addWidget(_minY);
	layout->addWidget(new QLabel(") (", this));
	layout->addWidget(_maxX);
	layout->addWidget(new QLabel(", ", this));
	layout->addWidget(_maxY);
	layout->addWidget(new QLabel(")", this));

	connect(_scene, &Scene::rectUpdated, this, &InfoWidget::updateData);
	connect(_graphicsView, &GraphicsView::pointerPositionUpdated, this, &InfoWidget::updateData);

	connect(_minX, QOverload<int>::of(&QSpinBox::valueChanged), this, [this, commands]() {
		SceneCommand::resize(commands, _scene, { _minX->value(), _scene->min().y() }, _scene->max());
	});
	connect(_minY, QOverload<int>::of(&QSpinBox::valueChanged), this, [this, commands]() {
		SceneCommand::resize(commands, _scene, { _scene->min().x(), _minY->value() }, _scene->max());
	});
	connect(_maxX, QOverload<int>::of(&QSpinBox::valueChanged), this, [this, commands]() {
		SceneCommand::resize(commands, _scene, _scene->min(), { _maxX->value(), _scene->max().y() });
	});
	connect(_maxY, QOverload<int>::of(&QSpinBox::valueChanged), this, [this, commands]() {
		SceneCommand::resize(commands, _scene, _scene->min(), { _scene->max().x(), _maxY->value() });
	});
}

void InfoWidget::updateData() {
	const auto& pos = _graphicsView->pointerPosition();
	_labelPosition->setText(QString("%1, %2").arg(pos.x()).arg(pos.y()));

	const auto& min = _scene->min();
	const auto& max = _scene->max();
	_minX->setValue(min.x());
	_minY->setValue(min.y());
	_maxX->setValue(max.x());
	_maxY->setValue(max.y());
}
} // namespace eno
