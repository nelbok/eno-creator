#include "MainWindow.hpp"

#include <QCloseEvent>
#include <QComboBox>
#include <QCoreApplication>
#include <QLabel>
#include <QMenuBar>
#include <QMessageBox>
#include <QStatusBar>
#include <QSpinBox>
#include <QToolBar>
#include <QBoxLayout>

#include "data/Data.hpp"
#include "controller/MapAction.hpp"
#include "controller/Shortcuts.hpp"

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

void MainWindow::reset() {
	_depthSpinBox->setValue(_mapAction->depth());
	_penWidthSpinBox->setValue(_mapAction->penWidth());
	auto variant = QVariant::fromValue(_mapAction->zoom());
	_zoomComboBox->setCurrentIndex(_zoomComboBox->findData(variant));
	updateWindowTitle();
}

void MainWindow::updateWindowTitle() {
	assert(_graphicsView != nullptr);

	QString title = qApp->applicationName();

	title.append(" - ");
	title.append(_data->projectName());
	if (_data->isModified()) {
		title.append("*");
	}

	setWindowTitle(title);
}

void MainWindow::updateInfos() {
	const auto& pos = _graphicsView->pointerPosition();
	const auto& min = _data->min();
	const auto& max = _data->max();
	_infoLabel->setText(QString("Position: %1, %2 Scene: (%3, %4) (%5, %6)").arg(pos.x()).arg(pos.y()).arg(min.x()).arg(min.y()).arg(max.x()).arg(max.y()));
}

void MainWindow::showMessage(const QString& message) {
	statusBar()->showMessage(message, 2000);
}

void MainWindow::initUi() {
	_data = new Data(this);
	connect(_data, &Data::filePathUpdated, this, &MainWindow::updateWindowTitle);
	connect(_data, &Data::isModifiedUpdated, this, &MainWindow::updateWindowTitle);
	connect(_data, &Data::rectUpdated, this, &MainWindow::updateInfos);

	_mapAction = new MapAction(_data, this);

	_shortcuts = new Shortcuts(_mapAction, this);
	_shortcuts->initActions();
	connect(_shortcuts, &Shortcuts::updated, this, &MainWindow::reset);
	connect(_shortcuts, &Shortcuts::showMessage, this, &MainWindow::showMessage);

	_graphicsView = new GraphicsView(_mapAction, this);
	_graphicsView->init();
	connect(_graphicsView, &GraphicsView::pointerPositionUpdated, this, &MainWindow::updateInfos);
	setCentralWidget(_graphicsView);

	_infoLabel = new QLabel(this);
	statusBar()->addPermanentWidget(_infoLabel);

	setMinimumSize(1280, 780);

	initMainMenu();
	initTools();
	initLayers();
	initGenerate();

	menuBar()->addAction("About Qt", [this]() {
		QMessageBox::aboutQt(this, qApp->applicationName());
	});

	_data->reset();
	_mapAction->reset();
	_shortcuts->resetActions();
	reset();

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
	auto* action5 = _shortcuts->colorDialogAction();

	auto* menu = menuBar()->addMenu("Tools");
	menu->addAction(action1);
	menu->addAction(action2);
	menu->addAction(action3);
	menu->addAction(action4);
	menu->addAction(action5);

	auto* toolBar = addToolBar("Tools");
	toolBar->addAction(action1);
	toolBar->addAction(action2);
	toolBar->addAction(action3);
	toolBar->addAction(action4);
	toolBar->addSeparator();
	toolBar->addAction(action5);
}

void MainWindow::initLayers() {
	auto* wLevel = new QWidget(this);
	{
		auto* layout = new QHBoxLayout(wLevel);
		_depthSpinBox = new QSpinBox(wLevel);
		_depthSpinBox->setMinimum(MapAction::minDepth);
		_depthSpinBox->setMaximum(MapAction::maxDepth);
		connect(_depthSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), [this](int value) {
			this->_mapAction->setDepth(value);
			showMessage(QString("Depth changed to %1").arg(value));
		});
		layout->addWidget(_depthSpinBox);
		auto* label = new QLabel(wLevel);
		label->setPixmap(QPixmap(":items/depth.png"));
		layout->addWidget(label);
	}

	auto* wPen = new QWidget(this);
	{
		auto* layout = new QHBoxLayout(wPen);
		_penWidthSpinBox = new QSpinBox(wPen);
		_penWidthSpinBox->setMinimum(MapAction::minPenWidth);
		_penWidthSpinBox->setMaximum(MapAction::maxPenWidth);
		connect(_penWidthSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), [this](int value) {
			this->_mapAction->setPenWidth(value);
			showMessage(QString("Pen width changed to %1pt").arg(value));
		});
		layout->addWidget(_penWidthSpinBox);
		auto* label = new QLabel(wPen);
		label->setPixmap(QPixmap(":items/width.png"));
		layout->addWidget(label);
	}

	auto* wZoom = new QWidget(this);
	{
		auto* layout = new QHBoxLayout(wZoom);
		_zoomComboBox = new QComboBox(wZoom);
		_zoomComboBox->addItem(MapAction::toString(MapAction::Zoom::x50), QVariant::fromValue(MapAction::Zoom::x50));
		_zoomComboBox->addItem(MapAction::toString(MapAction::Zoom::x100), QVariant::fromValue(MapAction::Zoom::x100));
		_zoomComboBox->addItem(MapAction::toString(MapAction::Zoom::x200), QVariant::fromValue(MapAction::Zoom::x200));
		_zoomComboBox->addItem(MapAction::toString(MapAction::Zoom::x400), QVariant::fromValue(MapAction::Zoom::x400));
		connect(_zoomComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [this](int index) {
			auto zoom = this->_zoomComboBox->itemData(index).value<MapAction::Zoom>();
			this->_mapAction->setZoom(zoom);
			showMessage(QString("Zoom changed to %1").arg(MapAction::toString(zoom)));
		});
		layout->addWidget(_zoomComboBox);
		auto* label = new QLabel(wZoom);
		label->setPixmap(QPixmap(":items/zoom.png"));
		layout->addWidget(label);
	};

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

} // namespace eno
