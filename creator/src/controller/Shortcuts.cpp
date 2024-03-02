#include "Shortcuts.hpp"

#include <QtGui/QAction>
#include <QtGui/QActionGroup>
#include <QtGui/QDesktopServices>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QStyle>

#include <eno/data/Project.hpp>
#include <eno/data/Scene.hpp>
#include <eno/io/Eno.hpp>

#include "engine/Engine.hpp"
#include "io/WavefrontOBJ.hpp"
#include "view/PreferencesWindow.hpp"
#include "Commands.hpp"
#include "Core.hpp"
#include "Graphics.hpp"
#include "Preferences.hpp"
#include "RecentFiles.hpp"

#include "Config.hpp"

namespace eno {

Shortcuts::Shortcuts(Core* core, QObject* parent)
	: QObject(parent)
	, _core(core) {}

void Shortcuts::initActions() {
	initFile();
	initEdit();
	initTools();
	initGenerate();
	initHelp();
}

void Shortcuts::resetActions() {
	assert(_core);
	switch (_core->graphics()->typeAction()) {
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
	auto* style = qApp->style();

	_newAction = new QAction("New file", this);
	_newAction->setIcon(style->standardIcon(QStyle::SP_FileIcon));
	_newAction->setShortcut({ Qt::CTRL | Qt::Key_N });
	connect(_newAction, &QAction::triggered, this, [this]() {
		if (!needToSave())
			return;
		_core->reset();
		resetActions();
		emit showMessage("New map created");
	});

	_openAction = new QAction("Open", this);
	_openAction->setIcon(style->standardIcon(QStyle::SP_DirOpenIcon));
	_openAction->setShortcut({ Qt::CTRL | Qt::Key_O });
	connect(_openAction, &QAction::triggered, this, [this]() {
		if (!needToSave())
			return;
		const auto& path = QFileDialog::getOpenFileName(qApp->activeWindow(), qApp->applicationName() + " - Open", Preferences::projectLocation(), Eno::fileType);
		if (path.isEmpty())
			return;

		open(path);
	});

	_saveAction = new QAction("Save", this);
	_saveAction->setIcon(style->standardIcon(QStyle::SP_DialogSaveButton));
	_saveAction->setShortcut({ Qt::CTRL | Qt::Key_S });
	connect(_saveAction, &QAction::triggered, this, [this]() {
		save(false);
	});

	_saveAsAction = new QAction("Save As", this);
	connect(_saveAsAction, &QAction::triggered, this, [this]() {
		save(true);
	});

	_recentFilesMenu = new QMenu("Recent files");
	auto rfLambda = [this]() {
		_recentFilesMenu->clear();
		for (const auto& data : _core->recentFiles()->datas()) {
			auto* action = new QAction(data, _recentFilesMenu);
			connect(action, &QAction::triggered, this, [this, data]() {
				if (!needToSave())
					return;

				open(data);
			});
			_recentFilesMenu->addAction(action);
		}
	};
	connect(_core->recentFiles(), &RecentFiles::datasUpdated, this, rfLambda);
	rfLambda();

	_preferenceAction = new QAction("Preferences", this);
	connect(_preferenceAction, &QAction::triggered, this, []() {
		auto* w = new PreferencesWindow;
		w->initUi();
		w->show();
	});

	_quitAction = new QAction("Quit", this);
	_quitAction->setShortcut({ Qt::CTRL | Qt::Key_Q });
	connect(_quitAction, &QAction::triggered, this, [this]() {
		if (!needToSave())
			return;
		_core->reset();
		qApp->exit();
	});
}

void Shortcuts::initEdit() {
	_undoAction = new QAction("Undo", this);
	_undoAction->setShortcut({ Qt::CTRL | Qt::Key_Z });
	connect(_core->commands(), &Commands::updated, this, [this]() {
		_undoAction->setEnabled(_core->commands()->canUndo());
	});
	connect(_undoAction, &QAction::triggered, this, [this]() {
		_core->commands()->undo();
		emit showMessage("Action undone");
	});

	_redoAction = new QAction("Redo", this);
	_redoAction->setShortcut({ Qt::CTRL | Qt::Key_Y });
	connect(_core->commands(), &Commands::updated, this, [this]() {
		_redoAction->setEnabled(_core->commands()->canRedo());
	});
	connect(_redoAction, &QAction::triggered, this, [this]() {
		_core->commands()->redo();
		emit showMessage("Action redone");
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
		_core->graphics()->setTypeAction(Preferences::TypeAction::Remove);
		emit showMessage("Eraser tool selected");
	});
	_addAction = createTools(QIcon(":items/add.png"), "Pen tool", "To add an item on the map", Preferences::keyAdd(), [this]() {
		_core->graphics()->setTypeAction(Preferences::TypeAction::Add);
		emit showMessage("Add tool selected");
	});
	_pickerAction = createTools(QIcon(":items/picker.png"), "Picker tool", "To pick a color on the map", Preferences::keyPicker(), [this]() {
		_core->graphics()->setTypeAction(Preferences::TypeAction::Picker);
		emit showMessage("Picker tool selected");
	});
	_resizeAction = createTools(QIcon(":items/resize.png"), "Resizing tool", "To resize the map", Preferences::keyResize(), [this]() {
		_core->graphics()->setTypeAction(Preferences::TypeAction::Resize);
		emit showMessage("Resize tool selected");
	});
}

void Shortcuts::initGenerate() {
	_generateOBJAction = new QAction(QIcon(":export/generate.png"), "Generate OBJ file", this);
	_generateOBJAction->setToolTip("Generate the OBJ file corresponding to the project");
	connect(_generateOBJAction, &QAction::triggered, this, [this]() {
		const auto& path =
			QFileDialog::getSaveFileName(qApp->activeWindow(), qApp->applicationName() + " - Export as", Preferences::generateLocation() + "/" + _core->project()->name(), WavefrontOBJ::fileType);
		if (path.isEmpty())
			return;

		auto* thread = new WavefrontOBJ(this);
		thread->init(_core->project(), IOThread::Type::Save, path);
		emit showProgressDialog(true, thread);
		connect(thread, &WavefrontOBJ::finished, this, [this, thread]() {
			emit showProgressDialog(false);
			const auto& name = _core->project()->name();
			switch (thread->result()) {
				case IOThread::Result::Success:
					emit showMessage(QString("Export [%1] successed").arg(name));
					break;
				case IOThread::Result::Canceled:
					emit showMessage(QString("Exporting [%1] canceled").arg(name));
					break;
				default:
					emit showMessage(QString("Failed to export %1").arg(name));
					break;
			}
			thread->deleteLater();
		});
		thread->start();
	});

	_generate3DAction = new QAction(QIcon(":export/opengl.png"), "Open 3D view", this);
	_generate3DAction->setToolTip("Show the project in the 3D view");
	_generate3DAction->setShortcut(Preferences::key3DView());
	connect(_generate3DAction, &QAction::triggered, this, [this]() {
		auto* widget = new Engine();
		widget->init(_core->project());
		widget->show();
		emit showMessage("Loading");
	});
}

void Shortcuts::initHelp() {
	_githubAction = new QAction("Github", this);
	connect(_githubAction, &QAction::triggered, []() {
		QDesktopServices::openUrl({ Config::github });
	});

	_twitterAction = new QAction("Twitter", this);
	connect(_twitterAction, &QAction::triggered, []() {
		QDesktopServices::openUrl({ Config::twitter });
	});

	_discordAction = new QAction("Discord", this);
	connect(_discordAction, &QAction::triggered, []() {
		QDesktopServices::openUrl({ Config::discord });
	});

	_aboutAction = new QAction("About " + QString(Config::creator), this);
	connect(_aboutAction, &QAction::triggered, []() {
		QString text = "<strong>About " + QString(Config::creator) + " - " + QString(Config::version) + "</strong><br/>";
		text += "<br/>";
		text += "3D map editor.<br/>";
		text += "<br/>";
		text += QString(Config::creator) + " is an easy application to allow you to create complex 3D scenes. You can export your project into Wavefront OBJ file.<br/>";
		text += "<br/>";
		text += QString(Config::creator) + " is licensed under GNU General Public License v3.0.<br/>";
		text += "<br/>";
		text += QString(Config::copyright);
		QMessageBox::about(qApp->activeWindow(), qApp->applicationName(), text);
	});

	_aboutQtAction = new QAction("About Qt", this);
	connect(_aboutQtAction, &QAction::triggered, []() {
		QMessageBox::aboutQt(qApp->activeWindow(), qApp->applicationName());
	});
}

bool Shortcuts::needToSave() {
	if (_core->project()->isModified()) {
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

void Shortcuts::open(const QString& path) {
	_core->reset();
	resetActions();
	auto* thread = new Eno(this);
	thread->init(_core->project(), IOThread::Type::Load, path);
	emit showProgressDialog(true, thread);
	connect(thread, &Eno::finished, this, [this, thread, path]() {
		emit showProgressDialog(false);
		switch (thread->result()) {
			case IOThread::Result::Success:
				_core->recentFiles()->add(path);
				emit showMessage(QString("Project [%1] loaded").arg(_core->project()->name()));
				break;
			case IOThread::Result::Canceled:
				_core->reset();
				emit showMessage(QString("Loading project canceled"));
				break;
			default:
				_core->recentFiles()->remove(path);
				_core->reset();
				emit showMessage(QString("Failed to load %1").arg(path));
				break;
		}
		thread->deleteLater();
	});
	thread->start();
}

void Shortcuts::save(bool newPathRequested) {
	QString path = _core->project()->filePath();

	QFileInfo fileInfo(path);
	auto isFile = fileInfo.isFile();
	auto isWritable = fileInfo.isWritable();

	if (newPathRequested || !isFile || !isWritable) {
		QString currentPath = _core->project()->filePath();
		if (currentPath.isEmpty()) {
			currentPath = Preferences::projectLocation() + "/" + _core->project()->name();
		}
		path = QFileDialog::getSaveFileName(qApp->activeWindow(), qApp->applicationName() + " - Save as", currentPath, Eno::fileType);
	}
	if (path.isEmpty())
		return;

	auto* thread = new Eno(this);
	thread->init(_core->project(), IOThread::Type::Save, path);
	emit showProgressDialog(true, thread);
	connect(thread, &Eno::finished, this, [this, thread, path]() {
		emit showProgressDialog(false);
		const auto& name = _core->project()->name();
		switch (thread->result()) {
			case IOThread::Result::Success:
				_core->recentFiles()->add(path);
				emit showMessage(QString("Project [%1] saved").arg(name));
				break;
			case IOThread::Result::Canceled:
				emit showMessage(QString("Saving project [%1] canceled").arg(name));
				break;
			default:
				emit showMessage(QString("Failed to save %1").arg(name));
				break;
		}
		thread->deleteLater();
	});
	thread->start();
}

} // namespace eno
