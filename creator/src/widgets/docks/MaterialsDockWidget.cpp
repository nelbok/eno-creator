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
#include "widgets/common/PercentSlider.hpp"
#include "widgets/common/TextureBox.hpp"

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
		disconnect(_current, &Material::diffuseMapUpdated, this, &MaterialsDockWidget::updateForm);
		disconnect(_current, &Material::opacityUpdated, this, &MaterialsDockWidget::updateForm);
		disconnect(_current, &Material::opacityMapUpdated, this, &MaterialsDockWidget::updateForm);
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
			connect(_current, &Material::diffuseMapUpdated, this, &MaterialsDockWidget::updateForm);
			connect(_current, &Material::opacityUpdated, this, &MaterialsDockWidget::updateForm);
			connect(_current, &Material::opacityMapUpdated, this, &MaterialsDockWidget::updateForm);
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

void MaterialsDockWidget::initForm() {
	auto* w = _layout->parentWidget();

	_name = new QLineEdit(w);
	_diffuse = new ColorButton(w);
	_diffuseMap = new TextureBox(w);
	_opacity = new PercentSlider(w);
	_opacityMap = new TextureBox(w);
	_refCount = new QLabel(w);

	_diffuseMap->init(_core->project());
	_opacity->init();
	_opacityMap->init(_core->project());

	_form = new QWidget(w);
	auto* form = new QFormLayout(_form);
	form->addRow("Name:", _name);
	form->addRow("Diffuse:", _diffuse);
	form->addRow("Diffuse map:", _diffuseMap);
	form->addRow("Opacity:", _opacity);
	form->addRow("Opacity map:", _opacityMap);
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
	connect(_diffuseMap, &TextureBox::currentTextureChanged, [this](Texture* texture) {
		if (_current) {
			MaterialCommand::setDiffuseMap(_core->commands(), _current, texture);
			if (texture)
				emit showMessage(QString("Material's diffuse texture changed to %1").arg(texture->name()));
			else
				emit showMessage(QString("Material's diffuse texture changed to none"));
		}
	});
	connect(_opacity, &PercentSlider::valueChanged, [this](double value) {
		if (_current) {
			MaterialCommand::setOpacity(_core->commands(), _current, value);
			emit showMessage(QString("Material's opacity value changed to %1").arg(value));
		}
	});
	connect(_opacityMap, &TextureBox::currentTextureChanged, [this](Texture* texture) {
		if (_current) {
			MaterialCommand::setOpacityMap(_core->commands(), _current, texture);
			if (texture)
				emit showMessage(QString("Material's opacity texture changed to %1").arg(texture->name()));
			else
				emit showMessage(QString("Material's opacity texture changed to none"));
		}
	});
	updateForm();
}

void MaterialsDockWidget::updateForm() {
	if (_form == nullptr)
		return;

	QSignalBlocker sn(_name);
	QSignalBlocker sd(_diffuse);
	QSignalBlocker sdM(_diffuseMap);
	QSignalBlocker so(_opacity);
	QSignalBlocker soM(_opacityMap);

	_form->setEnabled(_current);
	if (_current) {
		_name->setText(_current->name());
		_diffuse->setColor(_current->diffuse());
		_diffuseMap->setCurrentTexture(_current->diffuseMap());
		_opacity->setValue(_current->opacity());
		_opacityMap->setCurrentTexture(_current->opacityMap());
		_refCount->setText(QString("Used by %1 object(s)").arg(_current->refCount()));
	} else {
		_name->setText("-");
		_diffuse->setColor({ 0, 0, 0 });
		_diffuseMap->setCurrentTexture(nullptr);
		_opacity->setValue(1.0);
		_opacityMap->setCurrentTexture(nullptr);
		_refCount->setText("");
	}
}
} // namespace eno
