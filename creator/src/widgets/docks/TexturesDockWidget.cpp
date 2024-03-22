#include "TexturesDockWidget.hpp"

#include <QtWidgets/QApplication>
#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMessageBox>

#include <eno/data/Project.hpp>
#include <eno/data/Texture.hpp>

#include "controller/command/TextureCommand.hpp"
#include "controller/Core.hpp"
#include "widgets/common/PixmapButton.hpp"

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
		disconnect(_current, &Texture::dataUpdated, this, &TexturesDockWidget::updateForm);
		disconnect(_current, &Texture::refCountUpdated, this, &TexturesDockWidget::updateForm);
		disconnect(_current, &Texture::invertXUpdated, this, &TexturesDockWidget::updateForm);
		disconnect(_current, &Texture::invertYUpdated, this, &TexturesDockWidget::updateForm);
	}

	_current = nullptr;
	const auto& current = currentListData();
	if (current.isValid()) {
		_current = current.value<Texture*>();

		// Connect Texture's signals
		if (_current) {
			connect(_current, &Texture::nameUpdated, this, &TexturesDockWidget::updateForm);
			connect(_current, &Texture::dataUpdated, this, &TexturesDockWidget::updateForm);
			connect(_current, &Texture::refCountUpdated, this, &TexturesDockWidget::updateForm);
			connect(_current, &Texture::invertXUpdated, this, &TexturesDockWidget::updateForm);
			connect(_current, &Texture::invertYUpdated, this, &TexturesDockWidget::updateForm);
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
	_data = new PixmapButton(w);
	_invertX = new QCheckBox("X", w);
	_invertY = new QCheckBox("Y", w);
	_refCount = new QLabel(w);

	auto* invert = new QHBoxLayout(w);
	invert->setContentsMargins({ 0, 0, 0, 0 });
	invert->addWidget(_invertX);
	invert->addWidget(_invertY);

	_form = new QWidget(w);
	auto* form = new QFormLayout(_form);
	form->addRow("Name:", _name);
	form->addRow("Data:", _data);
	form->addRow("Invert:", invert);
	form->addRow(_refCount);
	_layout->addWidget(_form);

	connect(_name, &QLineEdit::returnPressed, this, [this]() {
		if (_current) {
			const auto& name = _name->text();
			TextureCommand::setName(_core->commands(), _current, name);
			emit showMessage(QString("Texture's name changed to %1").arg(name));
		}
	});
	connect(_data, &PixmapButton::currentPixmapChanged, this, [this]() {
		if (_current) {
			const auto& pixmap = _data->pixmap();
			if (pixmap.height() > 256 || pixmap.width() > 256) {
				QMessageBox::warning(qApp->activeWindow(), qApp->applicationName(), "Image needs to be less than or equal 256x256.");
				updateForm();
				return;
			} else if (pixmap.height() != pixmap.width()) {
				QMessageBox::warning(qApp->activeWindow(), qApp->applicationName(), "Image needs to be square.");
				updateForm();
				return;
			}
			TextureCommand::setData(_core->commands(), _current, _data->pixmap());
			emit showMessage(QString("Texture's data changed"));
		}
	});
	connect(_invertX, &QCheckBox::checkStateChanged, this, [this]() {
		if (_current) {
			bool invertX = _invertX->isChecked();
			TextureCommand::setInvertX(_core->commands(), _current, invertX);
			emit showMessage(QString("Texture's invert X changed to %1").arg((invertX) ? "true" : "false"));
		}
	});
	connect(_invertY, &QCheckBox::checkStateChanged, this, [this]() {
		if (_current) {
			bool invertY = _invertY->isChecked();
			TextureCommand::setInvertY(_core->commands(), _current, invertY);
			emit showMessage(QString("Texture's invert Y changed to %1").arg((invertY) ? "true" : "false"));
		}
	});
	updateForm();
}

void TexturesDockWidget::updateForm() {
	if (_form == nullptr)
		return;

	_form->setEnabled(_current);
	if (_current) {
		_name->setText(_current->name());
		_data->setPixmap(_current->pixmap());
		_invertX->setChecked(_current->invertX());
		_invertY->setChecked(_current->invertY());
		_refCount->setText(QString("Used by %1 material(s)").arg(_current->refCount()));
	} else {
		_name->setText("-");
		_data->setPixmap(QPixmap(":/logo/logo.png"));
		_invertX->setChecked(false);
		_invertY->setChecked(false);
		_refCount->setText("");
	}
}
} // namespace eno
