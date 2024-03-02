#include "MaterialsDockWidget.hpp"

#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>

#include <eno/data/Material.hpp>
#include <eno/data/Project.hpp>

#include "controller/command/MaterialCommand.hpp"
#include "controller/Core.hpp"
#include "controller/Graphics.hpp"
#include "widgets/common/ColorButton.hpp"

namespace eno {
MaterialsDockWidget::MaterialsDockWidget(QWidget* parent)
	: BaseDockWidget(parent) {}

QString MaterialsDockWidget::dockName() const {
	return "Materials";
}

void MaterialsDockWidget::initContent() {
	initList("Add material", "Remove material");
	initForm();

	connect(_core->project(), &Project::materialsUpdated, this, &BaseDockWidget::updateListDatas);
}

void MaterialsDockWidget::addClicked() {
	_core->graphics()->setMaterial(_core->createMyMaterial());
	emit showMessage("Material added");
}

void MaterialsDockWidget::removeClicked() {
	const auto& current = currentListData();
	if (current.isValid()) {
		auto* material = current.value<Material*>();
		MaterialCommand::destroy(_core->commands(), material, _core->project());
		emit showMessage(QString("Material %1 removed").arg(material->name()));
	}
}

void MaterialsDockWidget::currentListDataChanged() {
	// Disconnect Material's signals
	if (_current && _core->project()->materials().contains(_current)) {
		disconnect(_current, &Material::nameUpdated, this, &MaterialsDockWidget::updateForm);
		disconnect(_current, &Material::diffuseUpdated, this, &MaterialsDockWidget::updateForm);
		disconnect(_current, &Material::refCountUpdated, this, &MaterialsDockWidget::updateForm);
		_current = nullptr;
	}

	const auto& current = currentListData();
	if (current.isValid()) {
		_current = current.value<Material*>();

		// Connect Material's signals
		if (_current) {
			connect(_current, &Material::nameUpdated, this, &MaterialsDockWidget::updateForm);
			connect(_current, &Material::diffuseUpdated, this, &MaterialsDockWidget::updateForm);
			connect(_current, &Material::refCountUpdated, this, &MaterialsDockWidget::updateForm);
		}
	}

	_core->graphics()->setMaterial(_current);
	updateForm();
}

void MaterialsDockWidget::currentListDataEdited() {
	const auto& current = currentListData();
	if (current.isValid()) {
		const auto& name = currentListName();
		auto* material = current.value<Material*>();
		MaterialCommand::setName(_core->commands(), material, name);
		emit showMessage(QString("Material's name changed to %1").arg(name));
	}
}

QList<QPair<QString, QVariant>> MaterialsDockWidget::datas() const {
	QList<QPair<QString, QVariant>> datas;
	for (const auto& material : _core->project()->materials()) {
		disconnect(material, &Material::nameUpdated, this, &BaseDockWidget::updateListDatas);
		datas.append({ material->name(), QVariant::fromValue(material) });
		connect(material, &Material::nameUpdated, this, &BaseDockWidget::updateListDatas);
	}
	return datas;
}

void MaterialsDockWidget::initForm() {
	auto* w = _layout->parentWidget();

	_name = new QLineEdit(w);
	_diffuse = new ColorButton(w);
	_refCount = new QLabel(w);

	auto* form = new QFormLayout;
	form->addRow("Name:", _name);
	form->addRow("Diffuse:", _diffuse);
	form->addRow(_refCount);
	_layout->addLayout(form);

	connect(_name, &QLineEdit::returnPressed, this, [this]() {
		const auto& name = _name->text();
		MaterialCommand::setName(_core->commands(), _current, name);
		emit showMessage(QString("Material's name changed to %1").arg(name));
	});
	connect(_diffuse, &ColorButton::currentColorChanged, this, [this](const QColor& color) {
		MaterialCommand::setDiffuse(_core->commands(), _current, color);
		emit showMessage(QString("Material's diffuse color changed to %1").arg(color.name()));
	});
}

void MaterialsDockWidget::updateForm() {
	assert(_current);
	if (_current) {
		_name->setText(_current->name());
		_diffuse->setColor(_current->diffuse());
		_refCount->setText(QString("Numbers of time used by a cube: %1").arg(_current->refCount()));
	} else {
		_name->setText("-");
		_diffuse->setColor({ 0, 0, 0 });
		_refCount->setText(QString("Numbers of time used by a cube: %1").arg(-1));
	}
}
} // namespace eno
