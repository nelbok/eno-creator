#include "Shortcuts.hpp"

#include <QAction>
#include <QActionGroup>
#include <QApplication>
#include <QFileDialog>
#include <QMessageBox>

#include "data/Project.hpp"
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
	initOthers();
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
			this->showMessage(QString("Project [%1] loaded").arg(this->_mapAction->project()->projectName()));
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
		if (_mapAction->project()->isModified()) {
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
}

void Shortcuts::initGenerate() {
	_generateOBJAction = new QAction(QIcon(":export/generate.png"), "Generate OBJ file", this);
	_generateOBJAction->setToolTip("Generate the OBJ file corresponding at the project in a file");
	connect(_generateOBJAction, &QAction::triggered, [this]() {
		const auto& path = QFileDialog::getSaveFileName(qApp->activeWindow(), qApp->applicationName() + " - Export as", _mapAction->project()->projectName(), WavefrontOBJ::fileType);
		if (path.isEmpty())
			return false;

		const auto& name = this->_mapAction->project()->projectName();
		if (WavefrontOBJ(this->_mapAction->project()).save(path)) {
			showMessage(QString("Export %1 successed").arg(name));
			return true;
		} else {
			showMessage(QString("Failed to export %1").arg(name));
			return false;
		}
	});

	_generate3DAction = new QAction(QIcon(":export/opengl.png"), "Open 3D view", this);
	_generate3DAction->setToolTip("Open the 3D view and show the current scene inside");
	connect(_generate3DAction, &QAction::triggered, [this]() {
		auto* widget = new Engine();
		widget->init(_mapAction->project()->scene());
		widget->show();
		this->showMessage("Work In Progress");
	});
}

void Shortcuts::initOthers() {
	_aboutQtAction = new QAction("About Qt", this);
	connect(_aboutQtAction, &QAction::triggered, [this]() {
		QMessageBox::aboutQt(qApp->activeWindow(), qApp->applicationName());
	});
}

bool Shortcuts::save(bool newPathRequested) {
	QString path = _mapAction->project()->filePath();

	QFileInfo fileInfo(path);
	auto isFile = fileInfo.isFile();
	auto isWritable = fileInfo.isWritable();

	if (newPathRequested || !isFile || !isWritable) {
		QString currentPath = _mapAction->project()->filePath();
		if (currentPath.isEmpty()) {
			currentPath = _mapAction->project()->projectName();
		}
		path = QFileDialog::getSaveFileName(qApp->activeWindow(), qApp->applicationName() + " - Save as", currentPath, Eno::fileType);
	}
	if (path.isEmpty())
		return false;

	const auto& name = this->_mapAction->project()->projectName();
	if (Eno(this->_mapAction).save(path)) {
		showMessage(QString("Project [%1] saved").arg(name));
		return true;
	} else {
		showMessage(QString("Failed to save %1").arg(name));
		return false;
	}
}

} // namespace eno
