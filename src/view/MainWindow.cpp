#include "MainWindow.hpp"

#include <QCloseEvent>
#include <QCoreApplication>
#include <QMenuBar>
#include <QProgressBar>
#include <QStatusBar>
#include <QToolBar>

#include "data/Project.hpp"
#include "controller/MapAction.hpp"
#include "controller/Shortcuts.hpp"

#include "widgets/ComboBoxWidget.hpp"
#include "widgets/InfoWidget.hpp"
#include "widgets/MaterialsDockWidget.hpp"
#include "widgets/SpinBoxWidget.hpp"
#include "GraphicsView.hpp"

namespace eno {

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent) {
	setWindowIcon(QIcon(":/logo/logo.png"));
}

void MainWindow::closeEvent(QCloseEvent* e) {
	e->ignore();
	_shortcuts->quitAction()->trigger();
}

void MainWindow::updateWindowTitle() {
	assert(_graphicsView != nullptr);

	QString title = qApp->applicationName();

	title.append(" - ");
	title.append(_project->projectName());
	if (_project->isModified()) {
		title.append("*");
	}

	setWindowTitle(title);
}

void MainWindow::showMessage(const QString& message) {
	statusBar()->showMessage(message, 2000);
}

void MainWindow::showProgressBar(bool visible) {
	if (visible) {
		if (!_progressBar) {
			_progressBar = new QProgressBar;
			_progressBar->setRange(0, 0);
		}
		statusBar()->addWidget(_progressBar);
		setEnabled(false);
	} else {
		statusBar()->removeWidget(_progressBar);
		if (_progressBar) {
			_progressBar->deleteLater();
			_progressBar = nullptr;
		}
		setEnabled(true);
	}
}


void MainWindow::initUi() {
	_project = new Project(this);
	_project->init();
	connect(_project, &Project::filePathUpdated, this, &MainWindow::updateWindowTitle);
	connect(_project, &Project::isModifiedUpdated, this, &MainWindow::updateWindowTitle);

	_mapAction = new MapAction(_project, this);

	_shortcuts = new Shortcuts(_mapAction, this);
	_shortcuts->initActions();
	connect(_shortcuts, &Shortcuts::showMessage, this, &MainWindow::showMessage);
	connect(_shortcuts, &Shortcuts::showProgressBar, this, &MainWindow::showProgressBar);

	_graphicsView = new GraphicsView(_mapAction, this);
	_graphicsView->init();
	setCentralWidget(_graphicsView);

	auto* infoWidget = new InfoWidget(this);
	infoWidget->init(_project->scene(), _graphicsView);
	statusBar()->addPermanentWidget(infoWidget);

	setMinimumSize(1280, 780);

	initMainMenu();
	initTools();
	initLayers();
	initGenerate();
	initDocks();

	menuBar()->addAction(_shortcuts->aboutQtAction());

	_mapAction->reset();
	_shortcuts->resetActions();

	showMessage("Ready!");
}
void MainWindow::initMainMenu() {
	auto* menuFile = menuBar()->addMenu("File");

	menuFile->addAction(_shortcuts->newAction());
	menuFile->addAction(_shortcuts->openAction());
	menuFile->addSeparator();
	menuFile->addAction(_shortcuts->saveAction());
	menuFile->addAction(_shortcuts->saveAsAction());
	menuFile->addSeparator();
	menuFile->addAction(_shortcuts->quitAction());
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
		wLevel->setRange(MapAction::minDepth, MapAction::maxDepth);
		connect(wLevel, &SpinBoxTool::valueChanged, [this](int value) {
			this->_mapAction->setDepth(value);
			showMessage(QString("Depth changed to %1").arg(value));
		});
		connect(_mapAction, &MapAction::depthUpdated, [this, wLevel]() {
			wLevel->setValue(this->_mapAction->depth());
		});
	}

	auto* wPen = new SpinBoxTool(this);
	{
		wPen->init(":items/width.png");
		wPen->setRange(MapAction::minPenWidth, MapAction::maxPenWidth);
		connect(wPen, &SpinBoxTool::valueChanged, [this](int value) {
			this->_mapAction->setPenWidth(value);
			showMessage(QString("Pen width changed to %1pt").arg(value));
		});
		connect(_mapAction, &MapAction::penWidthUpdated, [this, wPen]() {
			wPen->setValue(this->_mapAction->penWidth());
		});
	}

	auto* wZoom = new ComboBoxTool(this);
	{
		wZoom->init(":items/zoom.png");
		wZoom->addItem(MapAction::toString(MapAction::Zoom::x50), QVariant::fromValue(MapAction::Zoom::x50));
		wZoom->addItem(MapAction::toString(MapAction::Zoom::x100), QVariant::fromValue(MapAction::Zoom::x100));
		wZoom->addItem(MapAction::toString(MapAction::Zoom::x200), QVariant::fromValue(MapAction::Zoom::x200));
		wZoom->addItem(MapAction::toString(MapAction::Zoom::x400), QVariant::fromValue(MapAction::Zoom::x400));
		connect(wZoom, &ComboBoxTool::currentItemChanged, [this](const QVariant& item) {
			auto zoom = item.value<MapAction::Zoom>();
			this->_mapAction->setZoom(zoom);
			showMessage(QString("Zoom changed to %1").arg(MapAction::toString(zoom)));
		});
		connect(_mapAction, &MapAction::zoomUpdated, [this, wZoom]() {
			wZoom->setCurrentItem(QVariant::fromValue(this->_mapAction->zoom()));
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

void MainWindow::initDocks() {
	auto* menuDocks = menuBar()->addMenu("Views");

	auto* dock = new MaterialsDockWidget(this);
	dock->init(_project->materials(), _mapAction);
	connect(dock, &MaterialsDockWidget::showMessage, this, &MainWindow::showMessage);
	addDockWidget(Qt::LeftDockWidgetArea, dock);
	menuDocks->addAction(dock->toggleViewAction());
}

} // namespace eno
