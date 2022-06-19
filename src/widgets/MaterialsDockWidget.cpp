#include "MaterialsDockWidget.hpp"

#include "controller/MapAction.hpp"
#include "data/Material.hpp"
#include "data/Materials.hpp"

#include <QApplication>
#include <QBoxLayout>
#include <QColorDialog>
#include <QFormLayout>
#include <QLabel>
#include <QListWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QToolBar>

namespace eno {
MaterialsDockWidget::MaterialsDockWidget(QWidget* parent)
	: QDockWidget(parent) {}

void MaterialsDockWidget::init(Materials* materials, MapAction* mapAction) {
	assert(materials);
	assert(mapAction);

	_materials = materials;
	_mapAction = mapAction;

	setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	setWindowTitle("Materials");

	auto* w = new QWidget(this);
	auto* l = new QVBoxLayout(w);

	auto* tb = new QToolBar(w);
	tb->addAction("Add material", [this]() {
		auto* material = this->_materials->createMaterial();
		material->setName("New material");
		this->_materials->add(material);
	});
	tb->addAction("Remove material", [this]() {
		auto* material = this->currentMaterialSelected();
		if (this->_materials->canRemove(material)) {
			this->_materials->remove(material);
		}
	});

	_list = new QListWidget(w);
	_name = new QLineEdit(w);
	_diffuse = new QPushButton(w);
	_refCount = new QLabel(w);

	_list->setSortingEnabled(true);

	auto* form = new QFormLayout(w);
	form->addRow("Name:", _name);
	form->addRow("Diffuse:", _diffuse);
	form->addRow(_refCount);

	l->addWidget(tb);
	l->addWidget(_list);
	l->addLayout(form);

	setWidget(w);

	connect(_mapAction, &MapAction::materialUpdated, [this]() {
		for (int row = 0; row < this->_list->count(); ++row) {
			auto* item = this->_list->item(row);
			auto* material = item->data(Qt::UserRole).value<Material*>();
			if (_mapAction->material() == material) {
				this->_list->setCurrentItem(item);
			}
		}
	});
	connect(_materials, &Materials::updated, this, &MaterialsDockWidget::resetList);

	connect(_list, &QListWidget::itemSelectionChanged, this, &MaterialsDockWidget::updateForm);
	connect(_list, &QListWidget::itemSelectionChanged, [this]() {
		auto* material = this->currentMaterialSelected();
		this->_mapAction->setMaterial(material);
	});

	connect(_name, &QLineEdit::returnPressed, [this]() {
		const auto& name = _name->text();
		this->_list->currentItem()->setText(name);
		this->currentMaterialSelected()->setName(name);
	});
	connect(_diffuse, &QPushButton::clicked, this, &MaterialsDockWidget::chooseDiffuse);
}

Material* MaterialsDockWidget::currentMaterialSelected() const {
	auto* item = _list->currentItem();
	return item->data(Qt::UserRole).value<Material*>();
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
	for (auto* material : *_materials) {
		auto* item = new QListWidgetItem;
		item->setText(material->name());
		auto data = QVariant::fromValue(material);
		item->setData(Qt::UserRole, data);
		_list->addItem(item);

		if (current == data) {
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
	auto* material = currentMaterialSelected();
	_name->setText(material->name());

	_diffuse->setText(material->diffuse().name());
	{
		auto image = QImage(48, 48, QImage::Format_RGB32);
		image.fill(material->diffuse());
		_diffuse->setIcon({ QPixmap::fromImage(image) });
	}

	_refCount->setText(QString("Numbers of time used by a cube: %1").arg(material->refCount()));
}

void MaterialsDockWidget::chooseDiffuse() {
	auto* material = this->currentMaterialSelected();
	QColorDialog* dialog = new QColorDialog(material->diffuse(), qApp->activeWindow());
	connect(dialog, &QColorDialog::currentColorChanged, [this, material](const QColor& color) {
		if (color.isValid()) {
			material->setDiffuse(color);
			this->showMessage(QString("Color changed to %1").arg(color.name()));
			updateForm();
		}
	});
	dialog->exec();
}

} // namespace eno
