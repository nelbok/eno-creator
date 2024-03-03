#include "TexturesDockWidget.hpp"

#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>

#include <eno/data/Project.hpp>
#include <eno/data/Texture.hpp>

#include "controller/command/TextureCommand.hpp"
#include "controller/Core.hpp"

namespace eno {
TexturesDockWidget::TexturesDockWidget(QWidget* parent)
	: BaseDockWidget(parent) {}

QString TexturesDockWidget::dockName() const {
	return "Textures";
}

void TexturesDockWidget::initContent() {
	initList("Add texture", "Remove texture");
	initForm();

	connect(_core->project(), &Project::texturesUpdated, this, &BaseDockWidget::updateListDatas);
}

void TexturesDockWidget::addClicked() {
	_core->createMyTexture();
	emit showMessage("Texture added");
}

void TexturesDockWidget::removeClicked() {
	const auto& current = currentListData();
	if (current.isValid()) {
		auto* texture = current.value<Texture*>();
		TextureCommand::destroy(_core->commands(), texture, _core->project());
		emit showMessage(QString("Texture %1 removed").arg(texture->name()));
	}
}

void TexturesDockWidget::currentListDataChanged() {
	// Disconnect Texture's signals
	if (_current && _core->project()->textures().contains(_current)) {
		disconnect(_current, &Texture::nameUpdated, this, &TexturesDockWidget::updateForm);
		disconnect(_current, &Texture::refCountUpdated, this, &TexturesDockWidget::updateForm);
		_current = nullptr;
	}

	const auto& current = currentListData();
	if (current.isValid()) {
		_current = current.value<Texture*>();

		// Connect Texture's signals
		if (_current) {
			connect(_current, &Texture::nameUpdated, this, &TexturesDockWidget::updateForm);
			connect(_current, &Texture::refCountUpdated, this, &TexturesDockWidget::updateForm);
		}
	}

	updateForm();
}

void TexturesDockWidget::currentListDataEdited() {
	const auto& current = currentListData();
	if (current.isValid()) {
		const auto& name = currentListName();
		auto* texture = current.value<Texture*>();
		TextureCommand::setName(_core->commands(), texture, name);
		emit showMessage(QString("Texture's name changed to %1").arg(name));
	}
}

QList<QPair<QString, QVariant>> TexturesDockWidget::datas() const {
	QList<QPair<QString, QVariant>> datas;
	const auto& textures = _core->project()->textures();
	for (const auto& texture : textures) {
		disconnect(texture, &Texture::nameUpdated, this, &BaseDockWidget::updateListDatas);
		datas.append({ texture->name(), QVariant::fromValue(texture) });
		connect(texture, &Texture::nameUpdated, this, &BaseDockWidget::updateListDatas);
	}
	return datas;
}

void TexturesDockWidget::initForm() {
	auto* w = _layout->parentWidget();

	_name = new QLineEdit(w);
	_refCount = new QLabel(w);

	auto* form = new QFormLayout;
	form->addRow("Name:", _name);
	form->addRow(_refCount);
	_layout->addLayout(form);

	connect(_name, &QLineEdit::returnPressed, this, [this]() {
		if (_current) {
			const auto& name = _name->text();
			TextureCommand::setName(_core->commands(), _current, name);
			emit showMessage(QString("Texture's name changed to %1").arg(name));
		}
	});
}

void TexturesDockWidget::updateForm() {
	assert(_current);
	if (_current) {
		_name->setText(_current->name());
		_refCount->setText(QString("Used by %1 material(s)").arg(_current->refCount()));
	} else {
		_name->setText("-");
		_refCount->setText(QString("Used by %1 material(s)").arg(-1));
	}
}
} // namespace eno
