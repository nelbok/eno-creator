#include "Shortcuts.hpp"

#include <QAction>
#include <QActionGroup>
#include <QApplication>
#include <QColorDialog>

#include "MapAction.hpp"

namespace eno {

Shortcuts::Shortcuts(MapAction* mapAction, QObject* parent)
	: QObject(parent)
	, _mapAction(mapAction) {}


void Shortcuts::initActions() {
	initFile();
	initTools();
}

void Shortcuts::resetActions() {
	assert(_mapAction);
	switch (_mapAction->typeAction()) {
		case MapAction::TypeAction::Remove:
			_removeAction->setChecked(true);
			break;
		case MapAction::TypeAction::Add:
			_addAction->setChecked(true);
			break;
		case MapAction::TypeAction::Picker:
			_pickerAction->setChecked(true);
			break;
		case MapAction::TypeAction::Resize:
			_resizeAction->setChecked(true);
			break;
	}
	updateColorDialogIcon();
}

void Shortcuts::updateColorDialogIcon() {
	assert(_colorDialogAction);
	auto image = QImage(48, 48, QImage::Format_RGB32);
	image.fill(_mapAction->color());
	_colorDialogAction->setIcon({ QPixmap::fromImage(image) });
}

void Shortcuts::initFile() {
	_newAction = new QAction("New file", this);
	connect(_newAction, &QAction::triggered, [this]() {
		this->_mapAction->reset();
		this->resetActions();
		this->updateWindowTitle();
		this->showMessage("New map created");
		this->updated();
	});


	_quitAction = new QAction("Quit", this);
	connect(_quitAction, &QAction::triggered, [this]() {
		//	CreatorSettings().do_save_recent_files(recent_files_);
		//	CreatorSettings().do_save_eno_directory(eno_directory_);
		//	CreatorSettings().do_save_obj_directory(obj_directory_);
		//
		//	if (creator_.map().is_modified())
		//		switch (QMessageBox::warning(this, QApplication::translate("CreatorWindow", "Shadow Revival Project - Creator"),
		//			QApplication::translate("CreatorWindow", "The map has been modified.\nDo you want to save your changes?"), QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel, QMessageBox::Save)) {
		//			case QMessageBox::Save:
		//				if (!do_save_triggered())
		//				default:
		//				case QMessageBox::Discard:
		//					qApp->exit();
		//				break;
		//			case QMessageBox::Cancel:
		//				break;
		//		}
		//	else
		qApp->exit();
	});
}

void Shortcuts::initTools() {
	auto* group = new QActionGroup(this);

	auto createTools = [this, group](const QIcon& icon, const QString& title, const QString& toolTip, std::function<void(void)> lambda) -> QAction* {
		auto* action = new QAction(icon, title, this);
		action->setToolTip(toolTip);
		action->setCheckable(true);
		action->setIconVisibleInMenu(false);
		connect(action, &QAction::triggered, lambda);
		group->addAction(action);
		return action;
	};
	_removeAction = createTools(QIcon(":items/remove.png"), "Eraser tool", "Select the tool to erase an item", [this]() {
		this->_mapAction->setTypeAction(MapAction::TypeAction::Remove);
		this->showMessage("Eraser tool selected");
	});
	_addAction = createTools(QIcon(":items/add.png"), "Pen tool", "Select the tool to add an item", [this]() {
		this->_mapAction->setTypeAction(MapAction::TypeAction::Add);
		this->showMessage("Add tool selected");
	});
	_pickerAction = createTools(QIcon(":items/picker.png"), "Picker tool", "Select the tool to pick a color on the map", [this]() {
		this->_mapAction->setTypeAction(MapAction::TypeAction::Picker);
		this->showMessage("Picker tool selected");
	});
	_resizeAction = createTools(QIcon(":items/resize.png"), "Resizing tool", "Select the tool to resize the map, warning, downgrade the size might erase a portion of the map!", [this]() {
		this->_mapAction->setTypeAction(MapAction::TypeAction::Resize);
		this->showMessage("Resize tool selected");
	});

	_colorDialogAction = new QAction("Open color dialog", this);
	_colorDialogAction->setToolTip("Open the color dialog so that you can choose the pen's color");
	connect(_colorDialogAction, &QAction::triggered, [this]() {
		QColor color = QColorDialog::getColor(this->_mapAction->color(), qApp->activeWindow());
		if (color.isValid()) {
			this->_mapAction->setColor(color);
			this->showMessage(QString("Color changed to %1").arg(color.name()));
			updateColorDialogIcon();
		}
	});
	connect(_mapAction, &MapAction::colorUpdated, this, &Shortcuts::updateColorDialogIcon);
	updateColorDialogIcon();
}
} // namespace eno
