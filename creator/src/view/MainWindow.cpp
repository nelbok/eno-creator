#include "MainWindow.hpp"

#include <QtCore/QCoreApplication>
#include <QtGui/QCloseEvent>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QProgressDialog>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>

#include <eno/data/Project.hpp>
#include <eno/io/IOThread.hpp>

#include "controller/Core.hpp"
#include "controller/Graphics.hpp"
#include "controller/Preferences.hpp"
#include "controller/Shortcuts.hpp"
#include "widgets/tools/ComboBoxTool.hpp"
#include "widgets/tools/SpinBoxTool.hpp"
#include "widgets/InfoWidget.hpp"
#include "widgets/MaterialsDockWidget.hpp"
#include "widgets/ProjectDockWidget.hpp"
#include "GraphicsView.hpp"

namespace eno {

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent) {}

void MainWindow::closeEvent(QCloseEvent* e) {
	e->ignore();
	_shortcuts->quitAction()->trigger();
}

void MainWindow::updateWindowTitle() {
	assert(_graphicsView != nullptr);

	QString title = qApp->applicationName();

	title.append(" - ");
	title.append(_project->name());
	if (_project->isModified()) {
		title.append("*");
	}

	setWindowTitle(title);
}

void MainWindow::showMessage(const QString& message) {
	statusBar()->showMessage(message, 2000);
}

void MainWindow::showProgressDialog(bool visible, IOThread* thread) {
	if (visible) {
		assert(!_progressDialog);
		assert(thread);
		_progressDialog = new QProgressDialog;
		_progressDialog->setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint);
		_progressDialog->setWindowModality(Qt::ApplicationModal);
		_progressDialog->setFixedSize(250, 100);
		_progressDialog->setRange(0, 0);
		connect(_progressDialog, &QProgressDialog::canceled, this, [thread]() {
			thread->requestInterruption();
		});
		_progressDialog->show();
		setEnabled(false);
	} else {
		assert(_progressDialog);
		_progressDialog->deleteLater();
		_progressDialog = nullptr;
		setEnabled(true);
	}
}

void MainWindow::initUi() {
	_project = new Project(this);
	_project->init();
	connect(_project, &Project::filePathUpdated, this, &MainWindow::updateWindowTitle);
	connect(_project, &Project::isModifiedUpdated, this, &MainWindow::updateWindowTitle);

	_core = new Core(_project, this);

	_shortcuts = new Shortcuts(_core, this);
	_shortcuts->initActions();
	connect(_shortcuts, &Shortcuts::showMessage, this, &MainWindow::showMessage);
	connect(_shortcuts, &Shortcuts::showProgressDialog, this, &MainWindow::showProgressDialog);

	_graphicsView = new GraphicsView(_core, this);
	_graphicsView->init();
	setCentralWidget(_graphicsView);

	auto* infoWidget = new InfoWidget(this);
	infoWidget->init(_core->commands(), _project->scene(), _graphicsView);
	statusBar()->addPermanentWidget(infoWidget);

	setMinimumSize(1280, 780);

	initFile();
	initEdit();
	initTools();
	initLayers();
	initGenerate();
	initDocks();
	initHelp();

	_core->reset();
	_shortcuts->resetActions();

	showMessage("Ready!");
}

void MainWindow::initFile() {
	auto* menu = menuBar()->addMenu("File");

	menu->addAction(_shortcuts->newAction());
	menu->addAction(_shortcuts->openAction());
	menu->addSeparator();
	menu->addMenu(_shortcuts->recentFilesMenu());
	menu->addSeparator();
	menu->addAction(_shortcuts->saveAction());
	menu->addAction(_shortcuts->saveAsAction());
	menu->addSeparator();
	menu->addAction(_shortcuts->preferenceAction());
	menu->addSeparator();
	menu->addAction(_shortcuts->quitAction());
}

void MainWindow::initEdit() {
	auto* menu = menuBar()->addMenu("Edit");
	menu->addAction(_shortcuts->undoAction());
	menu->addAction(_shortcuts->redoAction());
}

void MainWindow::initTools() {
	auto* action1 = _shortcuts->removeAction();
	auto* action2 = _shortcuts->addAction();
	auto* action3 = _shortcuts->pickerAction();
	auto* action4 = _shortcuts->resizeAction();

	auto* menu = menuBar()->addMenu("Tools");
	menu->addAction(action1);
	menu->addAction(action2);
	menu->addAction(action3);
	menu->addAction(action4);

	auto* toolBar = addToolBar("Tools");
	toolBar->addAction(action1);
	toolBar->addAction(action2);
	toolBar->addAction(action3);
	toolBar->addAction(action4);
}

void MainWindow::initLayers() {
	auto* wLevel = new SpinBoxTool(this);
	{
		wLevel->init(":items/depth.png");
		wLevel->setRange(Preferences::minDepth, Preferences::maxDepth);
		connect(wLevel, &SpinBoxTool::valueChanged, this, [this](int value) {
			_core->graphics()->setDepth(value);
			showMessage(QString("Depth changed to %1").arg(value));
		});
		connect(_core->graphics(), &Graphics::depthUpdated, this, [this, wLevel]() {
			wLevel->setValue(_core->graphics()->depth());
		});
	}

	auto* wPen = new SpinBoxTool(this);
	{
		wPen->init(":items/width.png");
		wPen->setRange(Preferences::minPenWidth, Preferences::maxPenWidth);
		connect(wPen, &SpinBoxTool::valueChanged, this, [this](int value) {
			_core->graphics()->setPenWidth(value);
			showMessage(QString("Pen width changed to %1pt").arg(value));
		});
		connect(_core->graphics(), &Graphics::penWidthUpdated, this, [this, wPen]() {
			wPen->setValue(_core->graphics()->penWidth());
		});
	}

	auto* wZoom = new ComboBoxTool(this);
	{
		wZoom->init(":items/zoom.png");
		wZoom->addItem(Preferences::toString(Preferences::Zoom::x25), QVariant::fromValue(Preferences::Zoom::x25));
		wZoom->addItem(Preferences::toString(Preferences::Zoom::x50), QVariant::fromValue(Preferences::Zoom::x50));
		wZoom->addItem(Preferences::toString(Preferences::Zoom::x100), QVariant::fromValue(Preferences::Zoom::x100));
		wZoom->addItem(Preferences::toString(Preferences::Zoom::x200), QVariant::fromValue(Preferences::Zoom::x200));
		connect(wZoom, &ComboBoxTool::currentItemChanged, this, [this](const QVariant& item) {
			auto zoom = item.value<Preferences::Zoom>();
			_core->graphics()->setZoom(zoom);
			showMessage(QString("Zoom changed to %1").arg(Preferences::toString(zoom)));
		});
		connect(_core->graphics(), &Graphics::zoomUpdated, this, [this, wZoom]() {
			wZoom->setCurrentItem(QVariant::fromValue(_core->graphics()->zoom()));
		});
	}

	auto* toolBar = addToolBar("Layers");
	toolBar->addWidget(wLevel);
	toolBar->addWidget(wPen);
	toolBar->addWidget(wZoom);
}

void MainWindow::initGenerate() {
	auto* action1 = _shortcuts->generateOBJAction();
	auto* action2 = _shortcuts->generate3DAction();

	auto* menu = menuBar()->addMenu("Generate");
	menu->addAction(action1);
	menu->addAction(action2);

	auto* toolBar = addToolBar("Generate");
	toolBar->addAction(action1);
	toolBar->addAction(action2);
}

void MainWindow::initHelp() {
	auto* menu = menuBar()->addMenu("Help");
	menu->addAction(_shortcuts->githubAction());
	menu->addAction(_shortcuts->twitterAction());
	menu->addAction(_shortcuts->discordAction());
	menu->addSeparator();
	menu->addAction(_shortcuts->aboutAction());
	menu->addSeparator();
	menu->addAction(_shortcuts->aboutQtAction());
}

void MainWindow::initDocks() {
	auto* menuDocks = menuBar()->addMenu("Views");

	{
		auto* dock = new ProjectDockWidget(this);
		dock->init(_core);
		connect(dock, &ProjectDockWidget::showMessage, this, &MainWindow::showMessage);
		addDockWidget(Qt::LeftDockWidgetArea, dock);
		menuDocks->addAction(dock->toggleViewAction());
	}

	{
		auto* dock = new MaterialsDockWidget(this);
		dock->init(_core);
		connect(dock, &MaterialsDockWidget::showMessage, this, &MainWindow::showMessage);
		addDockWidget(Qt::LeftDockWidgetArea, dock);
		menuDocks->addAction(dock->toggleViewAction());
	}
}

} // namespace eno
