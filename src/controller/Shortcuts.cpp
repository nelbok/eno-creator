#include "Shortcuts.hpp"

#include <QAction>
#include <QActionGroup>
#include <QApplication>
#include <QFileDialog>
#include <QMessageBox>

#include "data/Materials.hpp"
#include "data/Preferences.hpp"
#include "data/Project.hpp"
#include "data/Scene.hpp"
#include "engine/Engine.hpp"
#include "io/Eno.hpp"
#include "io/WavefrontOBJ.hpp"
#include "view/PreferencesWindow.hpp"
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
		case Preferences::TypeAction::Remove:
			_removeAction->setChecked(true);
			break;
		case Preferences::TypeAction::Add:
			_addAction->setChecked(true);
			break;
		case Preferences::TypeAction::Picker:
			_pickerAction->setChecked(true);
			break;
		case Preferences::TypeAction::Resize:
			_resizeAction->setChecked(true);
			break;
	}
}

void Shortcuts::initFile() {
	_newAction = new QAction("New file", this);
	connect(_newAction, &QAction::triggered, [this]() {
		if (!needToSave())
			return;
		this->_mapAction->reset();
		this->resetActions();
		this->showMessage("New map created");
		this->updated();
	});

	_openAction = new QAction("Open", this);
	connect(_openAction, &QAction::triggered, [this]() {
		if (!needToSave())
			return;
		const auto& path = QFileDialog::getOpenFileName(qApp->activeWindow(), qApp->applicationName() + " - Open", Preferences::projectLocation(), Eno::fileType);
		if (path.isEmpty())
			return;

		auto* thread = new Eno(this->_mapAction->project(), path, IOThread::Type::Load);
		showProgressDialog(true, thread);
		connect(thread, &Eno::finished, this, [this, thread, path]() {
			showProgressDialog(false);
			// Project has been loaded or it failed!
			this->_mapAction->project()->materials()->updated();
			this->_mapAction->project()->scene()->rectUpdated();
			this->_mapAction->project()->scene()->dataUpdated();
			switch (thread->result()) {
				case IOThread::Result::Success:
					showMessage(QString("Project [%1] loaded").arg(this->_mapAction->project()->projectName()));
					break;
				case IOThread::Result::Canceled:
					showMessage(QString("Loading project [%1] canceled").arg(this->_mapAction->project()->projectName()));
					break;
				default:
					showMessage(QString("Failed to load %1").arg(path));
					break;
			}
			thread->deleteLater();
		});
		thread->start();
	});

	_saveAction = new QAction("Save", this);
	connect(_saveAction, &QAction::triggered, [this]() {
		save(false);
	});

	_saveAsAction = new QAction("Save As", this);
	connect(_saveAsAction, &QAction::triggered, [this]() {
		save(true);
	});

	_preferenceAction = new QAction("Preferences", this);
	connect(_preferenceAction, &QAction::triggered, [this]() {
		auto* w = new PreferencesWindow;
		w->initUi();
		w->show();
	});

	_quitAction = new QAction("Quit", this);
	connect(_quitAction, &QAction::triggered, [this]() {
		if (!needToSave())
			return;
		qApp->exit();
	});
}

void Shortcuts::initTools() {
	auto* group = new QActionGroup(this);

	auto createTools = [this, group](const QIcon& icon, const QString& title, const QString& toolTip, const QKeySequence& shortcut, std::function<void(void)> lambda) -> QAction* {
		auto* action = new QAction(icon, title, this);
		action->setToolTip(toolTip);
		action->setShortcut(shortcut);
		action->setCheckable(true);
		action->setIconVisibleInMenu(false);
		connect(action, &QAction::triggered, lambda);
		group->addAction(action);
		return action;
	};
	_removeAction = createTools(QIcon(":items/remove.png"), "Eraser tool", "To erase an item on the map", Preferences::keyRemove(), [this]() {
		this->_mapAction->setTypeAction(Preferences::TypeAction::Remove);
		this->showMessage("Eraser tool selected");
	});
	_addAction = createTools(QIcon(":items/add.png"), "Pen tool", "To add an item on the map", Preferences::keyAdd(), [this]() {
		this->_mapAction->setTypeAction(Preferences::TypeAction::Add);
		this->showMessage("Add tool selected");
	});
	_pickerAction = createTools(QIcon(":items/picker.png"), "Picker tool", "To pick a color on the map", Preferences::keyPicker(), [this]() {
		this->_mapAction->setTypeAction(Preferences::TypeAction::Picker);
		this->showMessage("Picker tool selected");
	});
	_resizeAction = createTools(QIcon(":items/resize.png"), "Resizing tool", "To resize the map", Preferences::keyResize(), [this]() {
		this->_mapAction->setTypeAction(Preferences::TypeAction::Resize);
		this->showMessage("Resize tool selected");
	});
}

void Shortcuts::initGenerate() {
	_generateOBJAction = new QAction(QIcon(":export/generate.png"), "Generate OBJ file", this);
	_generateOBJAction->setToolTip("Generate the OBJ file corresponding to the project");
	connect(_generateOBJAction, &QAction::triggered, [this]() {
		const auto& path = QFileDialog::getSaveFileName(qApp->activeWindow(), qApp->applicationName() + " - Export as", Preferences::generateLocation(), WavefrontOBJ::fileType);
		if (path.isEmpty())
			return;

		auto* thread = new WavefrontOBJ(this->_mapAction->project(), path, IOThread::Type::Save);
		showProgressDialog(true, thread);
		connect(thread, &WavefrontOBJ::finished, this, [this, thread]() {
			showProgressDialog(false);
			const auto& name = this->_mapAction->project()->projectName();
			switch (thread->result()) {
				case IOThread::Result::Success:
					showMessage(QString("Export %1 successed").arg(name));
					break;
				case IOThread::Result::Canceled:
					showMessage(QString("Export %1 canceled").arg(name));
					break;
				default:
					showMessage(QString("Failed to export %1").arg(name));
					break;
			}
			thread->deleteLater();
		});
		thread->start();
	});

	_generate3DAction = new QAction(QIcon(":export/opengl.png"), "Open 3D view", this);
	_generate3DAction->setToolTip("Show the project in the 3D view");
	_generate3DAction->setShortcut(Preferences::key3DView());
	connect(_generate3DAction, &QAction::triggered, [this]() {
		auto* widget = new Engine();
		widget->init(_mapAction->project());
		widget->show();
		this->showMessage("Loading");
	});
}

void Shortcuts::initOthers() {
	_aboutQtAction = new QAction("About Qt", this);
	connect(_aboutQtAction, &QAction::triggered, [this]() {
		QMessageBox::aboutQt(qApp->activeWindow(), qApp->applicationName());
	});
}

bool Shortcuts::needToSave() {
	if (_mapAction->project()->isModified()) {
		const auto& button = QMessageBox::warning(qApp->activeWindow(), qApp->applicationName(), "The project has been modified.\nDo you want to save your changes?",
			QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel, QMessageBox::Save);
		switch (button) {
			case QMessageBox::Cancel:
				return false;
			case QMessageBox::Save:
				save(false);
				break;
			default:
				break;
		}
	}
	return true;
}

void Shortcuts::save(bool newPathRequested) {
	QString path = _mapAction->project()->filePath();

	QFileInfo fileInfo(path);
	auto isFile = fileInfo.isFile();
	auto isWritable = fileInfo.isWritable();

	if (newPathRequested || !isFile || !isWritable) {
		QString currentPath = _mapAction->project()->filePath();
		if (currentPath.isEmpty()) {
			currentPath = Preferences::projectLocation();
		}
		path = QFileDialog::getSaveFileName(qApp->activeWindow(), qApp->applicationName() + " - Save as", currentPath, Eno::fileType);
	}
	if (path.isEmpty())
		return;

	auto* thread = new Eno(this->_mapAction->project(), path, IOThread::Type::Save);
	showProgressDialog(true, thread);
	connect(thread, &Eno::finished, this, [this, thread, path]() {
		showProgressDialog(false);
		const auto& name = this->_mapAction->project()->projectName();
		if (thread->result() == IOThread::Result::Success) {
			showMessage(QString("Project [%1] saved").arg(name));
		} else {
			showMessage(QString("Failed to save %1").arg(name));
		}
		switch (thread->result()) {
			case IOThread::Result::Success:
				showMessage(QString("Project [%1] saved").arg(name));
				break;
			case IOThread::Result::Canceled:
				showMessage(QString("Savind project [%1] canceled").arg(name));
				break;
			default:
				showMessage(QString("Failed to save %1").arg(name));
				break;
		}
		thread->deleteLater();
	});
	thread->start();
}

} // namespace eno
