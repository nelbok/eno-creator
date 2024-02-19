#include "MaterialsDockWidget.hpp"

#include <QtWidgets/QApplication>
#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QToolBar>

#include <eno/data/Material.hpp>
#include <eno/data/Project.hpp>

#include "controller/MapAction.hpp"
#include "widgets/common/ColorButton.hpp"

namespace eno {
MaterialsDockWidget::MaterialsDockWidget(QWidget* parent)
	: QDockWidget(parent) {}

void MaterialsDockWidget::init(MapAction* mapAction) {
	assert(mapAction);
	_mapAction = mapAction;

	setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	setWindowTitle("Materials");

	auto* w = new QWidget(this);
	auto* l = new QVBoxLayout(w);

	auto* tb = new QToolBar(w);
	tb->addAction("Add material", [this]() {
		auto* material = new Material(_mapAction->project());
		material->setName("New material");
		_mapAction->project()->add(material);
		_mapAction->setMaterial(material);
	});
	tb->addAction("Remove material", [this]() {
		if (_mapAction->project()->canRemove(_current)) {
			_mapAction->project()->remove(_current);
		}
	});

	_list = new QListWidget(w);
	_name = new QLineEdit(w);
	_diffuse = new ColorButton(w);
	_refCount = new QLabel(w);

	_list->setSortingEnabled(true);

	auto* form = new QFormLayout;
	form->addRow("Name:", _name);
	form->addRow("Diffuse:", _diffuse);
	form->addRow(_refCount);

	l->addWidget(tb);
	l->addWidget(_list);
	l->addLayout(form);

	setWidget(w);

	connect(_mapAction, &MapAction::materialUpdated, this, [this]() {
		for (int row = 0; row < _list->count(); ++row) {
			auto* item = _list->item(row);
			auto* material = item->data(Qt::UserRole).value<Material*>();
			if (_mapAction->material() == material) {
				_list->setCurrentItem(item);
			}
		}
	});
	connect(_mapAction->project(), &Project::materialsUpdated, this, &MaterialsDockWidget::resetList);

	connect(_list, &QListWidget::itemSelectionChanged, this, [this]() {
		// Disconnect Material's signals
		if (_current && _mapAction->project()->materials().contains(_current)) {
			disconnect(_current, &Material::nameUpdated, this, &MaterialsDockWidget::updateForm);
			disconnect(_current, &Material::diffuseUpdated, this, &MaterialsDockWidget::updateForm);
			disconnect(_current, &Material::refCountUpdated, this, &MaterialsDockWidget::updateForm);
		}

		_current = _list->currentItem()->data(Qt::UserRole).value<Material*>();
		_mapAction->setMaterial(_current);

		// Connect Material's signals
		if (_current) {
			connect(_current, &Material::nameUpdated, this, &MaterialsDockWidget::updateForm);
			connect(_current, &Material::diffuseUpdated, this, &MaterialsDockWidget::updateForm);
			connect(_current, &Material::refCountUpdated, this, &MaterialsDockWidget::updateForm);
		}

		updateForm();
	});

	connect(_name, &QLineEdit::returnPressed, this, [this]() {
		const auto& name = _name->text();
		_list->currentItem()->setText(name);
		_current->setName(name);
		emit showMessage(QString("Material's name changed to %1").arg(name));
	});
	connect(_diffuse, &ColorButton::currentColorChanged, this, [this](const QColor& color) {
		_current->setDiffuse(color);
		emit showMessage(QString("Material's diffuse color changed to %1").arg(color.name()));
	});
}

void MaterialsDockWidget::resetList() {
	assert(_list);
	QVariant current{};
	if (auto* item = _list->currentItem()) {
		current = item->data(Qt::UserRole);
	}
	QListWidgetItem* currentItem = nullptr;

	_list->blockSignals(true);
	_list->clear();
	for (auto* material : _mapAction->project()->materials()) {
		auto* item = new QListWidgetItem;
		item->setText(material->name());
		auto d = QVariant::fromValue(material);
		item->setData(Qt::UserRole, d);
		_list->addItem(item);

		if (current == d) {
			currentItem = item;
		}
	}
	_list->blockSignals(false);
	if (currentItem) {
		_list->setCurrentItem(currentItem);
	} else {
		_list->setCurrentItem(_list->item(0));
	}
}

void MaterialsDockWidget::updateForm() {
	_name->setText(_current->name());
	_diffuse->setColor(_current->diffuse());
	_refCount->setText(QString("Numbers of time used by a cube: %1").arg(_current->refCount()));
}

} // namespace eno
