#include "MaterialsDockWidget.hpp"

#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>

#include <eno/data/Material.hpp>
#include <eno/data/Project.hpp>
#include <eno/data/Texture.hpp>

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
	connect(_core->graphics(), &Graphics::materialUpdated, this, [this]() {
		auto* material = _core->graphics()->material();
		if (_core->graphics()->material() != _current) {
			setCurrentListData(QVariant::fromValue(material));
		}
	});
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
	}

	_current = nullptr;
	const auto& current = currentListData();
	if (current.isValid()) {
		_current = current.value<Material*>();

		// Connect Material's signals
		if (_current) {
			connect(_current, &Material::nameUpdated, this, &MaterialsDockWidget::updateForm);
			connect(_current, &Material::diffuseUpdated, this, &MaterialsDockWidget::updateForm);
			connect(_current, &Material::refCountUpdated, this, &MaterialsDockWidget::updateForm);

			_core->graphics()->setMaterial(_current);
		}
	}

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
	const auto& materials = _core->project()->materials();
	for (const auto& material : materials) {
		disconnect(material, &Material::nameUpdated, this, &BaseDockWidget::updateListDatas);
		datas.append({ material->name(), QVariant::fromValue(material) });
		connect(material, &Material::nameUpdated, this, &BaseDockWidget::updateListDatas);
	}
	return datas;
}

void MaterialsDockWidget::updateTextures(const QList<Texture*>& textures) {
	_texture->blockSignals(true);
	_texture->clear();

	// Default value
	_texture->addItem("None");
	_texture->setCurrentIndex(0);

	for (auto* texture : textures) {
		disconnect(texture, &Texture::nameUpdated, this, &MaterialsDockWidget::updateTextureName);
		_texture->addItem(texture->name(), QVariant::fromValue(texture));
		connect(texture, &Texture::nameUpdated, this, &MaterialsDockWidget::updateTextureName);
	}
	if (_current && _current->texture())
		_texture->setCurrentIndex(_texture->findData(QVariant::fromValue(_current->texture())));
	_texture->blockSignals(false);
}

void MaterialsDockWidget::updateTextureName() {
	assert(sender());
	auto* texture = qobject_cast<Texture*>(sender());
	assert(texture);
	int index = _texture->findData(QVariant::fromValue(texture));
	assert(index != -1);
	_texture->setItemText(index, texture->name());
}

void MaterialsDockWidget::initForm() {
	auto* w = _layout->parentWidget();

	_name = new QLineEdit(w);
	_diffuse = new ColorButton(w);
	_texture = new QComboBox(w);
	_refCount = new QLabel(w);

	_form = new QWidget(w);
	auto* form = new QFormLayout(_form);
	form->addRow("Name:", _name);
	form->addRow("Diffuse:", _diffuse);
	form->addRow("Texture:", _texture);
	form->addRow(_refCount);
	_layout->addWidget(_form);

	connect(_name, &QLineEdit::returnPressed, this, [this]() {
		if (_current) {
			const auto& name = _name->text();
			MaterialCommand::setName(_core->commands(), _current, name);
			emit showMessage(QString("Material's name changed to %1").arg(name));
		}
	});
	connect(_diffuse, &ColorButton::currentColorChanged, this, [this](const QColor& color) {
		if (_current) {
			MaterialCommand::setDiffuse(_core->commands(), _current, color);
			emit showMessage(QString("Material's diffuse color changed to %1").arg(color.name()));
		}
	});
	connect(_texture, &QComboBox::currentIndexChanged, [this](int index) {
		if (_current && index != -1) {
			auto* texture = _texture->itemData(index).value<Texture*>();
			MaterialCommand::setTexture(_core->commands(), _current, texture);
			if (texture)
				emit showMessage(QString("Material's texture changed to %1").arg(texture->name()));
			else
				emit showMessage(QString("Material's texture changed to none"));
		}
	});
	connect(_core->project(), &Project::texturesUpdated, this, &MaterialsDockWidget::updateTextures);
	updateTextures(_core->project()->textures());
	updateForm();
}

void MaterialsDockWidget::updateForm() {
	if (_form == nullptr)
		return;

	_form->setEnabled(_current);
	if (_current) {
		_name->setText(_current->name());
		_diffuse->setColor(_current->diffuse());
		if (_current->texture())
			_texture->setCurrentIndex(_texture->findData(QVariant::fromValue(_current->texture())));
		else
			_texture->setCurrentIndex(0);
		_refCount->setText(QString("Used by %1 object(s)").arg(_current->refCount()));
	} else {
		_name->setText("-");
		_diffuse->setColor({ 0, 0, 0 });
		_texture->setCurrentIndex(0);
		_refCount->setText("");
	}
}
} // namespace eno
