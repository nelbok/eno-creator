#include "Shortcuts.hpp"

#include <QAction>
#include <QActionGroup>
#include <QApplication>
#include <QColorDialog>
#include <QFileDialog>
#include <QMessageBox>

#include "data/Data.hpp"
#include "engine/Engine.hpp"
#include "io/Eno.hpp"
#include "io/WavefrontOBJ.hpp"
#include "MapAction.hpp"

namespace eno {

Shortcuts::Shortcuts(MapAction* mapAction, QObject* parent)
	: QObject(parent)
	, _mapAction(mapAction) {}


void Shortcuts::initActions() {
	initFile();
	initTools();
	initGenerate();
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
		this->showMessage("New map created");
		this->updated();
	});

	_openAction = new QAction("Open", this);
	connect(_openAction, &QAction::triggered, [this]() {
		const auto& path = QFileDialog::getOpenFileName(qApp->activeWindow(), qApp->applicationName() + " - Open", "", Eno::fileType);
		if (path.isEmpty())
			return;
		if (Eno(this->_mapAction).load(path)) {
			this->showMessage("Map %1 loaded");
		} else {
			this->showMessage(QString("Failed to load %1").arg(path));
		}
	});

	_saveAction = new QAction("Save", this);
	connect(_saveAction, &QAction::triggered, [this]() {
		save(false);
	});

	_saveAsAction = new QAction("Save As", this);
	connect(_saveAsAction, &QAction::triggered, [this]() {
		save(true);
	});

	_quitAction = new QAction("Quit", this);
	connect(_quitAction, &QAction::triggered, [this]() {
		if (_mapAction->data()->isModified()) {
			const auto& button = QMessageBox::warning(qApp->activeWindow(), qApp->applicationName(), "The map has been modified.\nDo you want to save your changes?",
				QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel, QMessageBox::Save);
			switch (button) {
				case QMessageBox::Save:
					save(false);
					break;
				case QMessageBox::Cancel:
					return;
				default:
					break;
			}
		}
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

void Shortcuts::initGenerate() {
	_generateOBJAction = new QAction(QIcon(":export/generate.png"), "Generate OBJ file", this);
	_generateOBJAction->setToolTip("Generate the OBJ file corresponding at the project in a file");
	connect(_generateOBJAction, &QAction::triggered, [this]() {
		const QString& path = QFileDialog::getSaveFileName(qApp->activeWindow(), qApp->applicationName() + " - Export as", "", WavefrontOBJ::fileType);
		if (path.isEmpty())
			return false;

		if (WavefrontOBJ(this->_mapAction).save(path)) {
			showMessage(QString("Export %1 successed").arg(path));
			return true;
		} else {
			showMessage(QString("Failed to export %1").arg(path));
			return false;
		}
	});

	_generate3DAction = new QAction(QIcon(":export/opengl.png"), "Open 3D view", this);
	_generate3DAction->setToolTip("Open the 3D view and show the current scene inside");
	connect(_generate3DAction, &QAction::triggered, [this]() {
		auto* widget = new Engine();
		widget->init(_mapAction->data());
		widget->show();
		this->showMessage("Work In Progress");
	});
}

bool Shortcuts::save(bool newPathRequested) {
	QString path = _mapAction->data()->filePath();

	QFileInfo fileInfo(path);
	auto isFile = fileInfo.isFile();
	auto isWritable = fileInfo.isWritable();

	if (newPathRequested || !isFile || !isWritable) {
		path = QFileDialog::getSaveFileName(qApp->activeWindow(), qApp->applicationName() + " - Save as", "", Eno::fileType);
	}
	if (path.isEmpty())
		return false;

	if (Eno(this->_mapAction).save(path)) {
		showMessage(QString("Map %1 saved").arg(path));
		return true;
	} else {
		showMessage(QString("Failed to save %1").arg(path));
		return false;
	}
}

} // namespace eno
