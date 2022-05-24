#include "MainWindow.hpp"

#include <QCloseEvent>
#include <QComboBox>
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

const QString appTitle{ "Eno Creator" };
MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent) {
	setWindowIcon(QIcon(":/logo/logo.png"));

	//ui_->mainToolBar->addSeparator();
	//generate_action_ = ui_->mainToolBar->addAction(QIcon(":export/generate.png"), "", this, SLOT(do_generate_clicked()));
	//opengl_action_ = ui_->mainToolBar->addAction(QIcon(":export/opengl.png"), "", this, SLOT(do_preview_clicked()));

	//ui_->mainToolBar->addSeparator();
	//ui_->mainToolBar->addWidget(&info_label_);
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
}

void MainWindow::updateWindowTitle() {
	assert(_graphicsView != nullptr);

	QString title = appTitle;

	//if (!_creator->map().file_name().isEmpty()) {
	//	title.prepend(" - ");
	//	if (_creator->map().is_modified())
	//		title.prepend("*");
	//	title.prepend(QFileInfo(_creator->map().file_name()).fileName());
	//}

	setWindowTitle(title);
}

void MainWindow::showMessage(const QString& message) {
	statusBar()->showMessage(message, 2000);
}

void MainWindow::initUi() {
	auto* data = new Data(this);
	data->reset();

	_mapAction = new MapAction(data, this);

	_shortcuts = new Shortcuts(_mapAction, this);
	_shortcuts->initActions();
	connect(_shortcuts, &Shortcuts::updated, this, &MainWindow::reset);
	connect(_shortcuts, &Shortcuts::updateWindowTitle, this, &MainWindow::updateWindowTitle);
	connect(_shortcuts, &Shortcuts::showMessage, this, &MainWindow::showMessage);

	_graphicsView = new GraphicsView(_mapAction, this);
	_graphicsView->init();
	setCentralWidget(_graphicsView);

	setMinimumSize(1280, 780);

	initMainMenu();
	initTools();
	initLayers();

	menuBar()->addAction("About Qt", [this]() {
		QMessageBox::aboutQt(this, appTitle);
	});

	_mapAction->reset();
	reset();

	showMessage("Ready!");
}
void MainWindow::initMainMenu() {
	auto* menuFile = menuBar()->addMenu("File");

	menuFile->addAction(_shortcuts->newAction());
	menuFile->addAction("TODO: Open");
	menuFile->addSeparator();
	menuFile->addAction("TODO: Save");
	menuFile->addAction("TODO: Save as");
	menuFile->addSeparator();
	menuFile->addAction("TODO: Preferences");
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
		_depthSpinBox->setMinimum(-100);
		_depthSpinBox->setMaximum(100);
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
		_penWidthSpinBox->setMinimum(1);
		_penWidthSpinBox->setMaximum(10);
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
		_zoomComboBox->addItem(MapAction::toString(MapAction::Zoom::x10), QVariant::fromValue(MapAction::Zoom::x10));
		_zoomComboBox->addItem(MapAction::toString(MapAction::Zoom::x25), QVariant::fromValue(MapAction::Zoom::x25));
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

//
//void CreatorWindow::do_init_settings() {
//	//Recent files
//	QMenu* m = ui_->recentFilesMenu;
//
//	m->clear();
//	QStringList list = CreatorSettings().do_load_recent_files();
//	for (int i = 0; i < list.size(); ++i) {
//		if (QFile::exists(list.at(i)))
//			m->addAction(list.at(i), this, SLOT(do_recent_files_triggered()));
//		recent_files_.append(list.at(i));
//	}
//
//	//Default color
//	QColor c = CreatorSettings().do_load_color();
//	if (c.isValid())
//		creator_.color(c);
//
//	//Default directory
//	eno_directory_ = CreatorSettings().do_load_eno_directory();
//	obj_directory_ = CreatorSettings().do_load_obj_directory();
//
//	//Default language
//	QTranslator* trans = new QTranslator();
//	trans->load(":/lang/" + CreatorSettings().do_load_language());
//	QApplication::installTranslator(trans);
//}
//
//void CreatorWindow::do_add_recent_file() {
//	QMenu* m = ui_->recentFilesMenu;
//
//	recent_files_.removeAll(creator_.map().file_name());
//	recent_files_.prepend(creator_.map().file_name());
//
//	m->clear();
//	int max = 5;
//	for (int i = 0; i < recent_files_.size() && i < max; ++i)
//		if (QFile::exists(recent_files_.at(i)))
//			m->addAction(recent_files_.at(i), this, SLOT(do_recent_files_triggered()));
//		else
//			++max;
//}
//
//
////File menu
//
//void CreatorWindow::do_open_triggered() {
//	creator_.reset();
//	CreatorEno ce(creator_.map(), eno_directory_);
//	if (ce.open(creator_)) {
//		ui_->statusBar->showMessage(QApplication::translate("CreatorWindow", "Opening the map completed."), TIME_STATUSBAR);
//		do_add_recent_file();
//	} else
//		ui_->statusBar->showMessage(QApplication::translate("CreatorWindow", "Error while opening the map."), TIME_STATUSBAR);
//}
//
//void CreatorWindow::do_recent_files_triggered() {
//	QAction* act = qobject_cast<QAction*>(sender());
//
//	creator_.reset();
//	CreatorEno ce(creator_.map(), eno_directory_);
//	if (ce.open(act->text(), creator_)) {
//		ui_->statusBar->showMessage(QApplication::translate("CreatorWindow", "Opening the map completed."), TIME_STATUSBAR);
//		do_add_recent_file();
//	} else
//		ui_->statusBar->showMessage(QApplication::translate("CreatorWindow", "Error while opening the map."), TIME_STATUSBAR);
//}
//
//
//bool CreatorWindow::do_save_triggered() {
//	CreatorEno ce(creator_.map(), eno_directory_);
//	if (ce.save()) {
//		ui_->statusBar->showMessage(QApplication::translate("CreatorWindow", "Saving the map completed."), TIME_STATUSBAR);
//		do_add_recent_file();
//		return true;
//	} else {
//		ui_->statusBar->showMessage(QApplication::translate("CreatorWindow", "Error while saving the map."), TIME_STATUSBAR);
//		return false;
//	}
//}
//
//void CreatorWindow::do_save_as_triggered() {
//	CreatorEno ce(creator_.map(), eno_directory_);
//	if (ce.save_as()) {
//		ui_->statusBar->showMessage(QApplication::translate("CreatorWindow", "Saving the map completed."), TIME_STATUSBAR);
//		do_add_recent_file();
//	} else
//		ui_->statusBar->showMessage(QApplication::translate("CreatorWindow", "Error while saving the map."), TIME_STATUSBAR);
//}
//
//
//void CreatorWindow::do_generate_triggered() {
//	CreatorWavefront cw;
//	if (cw.generate(creator_.map(), obj_directory_))
//		ui_->statusBar->showMessage(QApplication::translate("CreatorWindow", "Generating of the Wavefront file completed."), TIME_STATUSBAR);
//	else
//		ui_->statusBar->showMessage(QApplication::translate("CreatorWindow", "Error while generating of the Wavefront file."), TIME_STATUSBAR);
//}
//
//
//void CreatorWindow::do_preference_triggered() {
//	CreatorPreference* obj = new CreatorPreference();
//	connect(obj, SIGNAL(sig_settings_updated()), SLOT(do_init_settings()));
//	obj->show();
//	ui_->statusBar->showMessage(QApplication::translate("CreatorWindow", "Open preference completed."), TIME_STATUSBAR);
//}
//
//
//void CreatorWindow::do_preview_clicked() {
//	(new opengl(creator_.map()))->show();
//	ui_->statusBar->showMessage(QApplication::translate("CreatorWindow", "Show preview completed."), TIME_STATUSBAR);
//}
//
//
//void CreatorWindow::do_update_info() {
//	info_label_.setText(QString("%1, %2 (%3, %4) (%5, %6)")
//												.arg((creator_.pos_x() / 10))
//												.arg(-(creator_.pos_y() / 10) - 1)
//												.arg(creator_.map().x_min() / 10)
//												.arg(creator_.map().y_min() / 10)
//												.arg(creator_.map().x_max() / 10)
//												.arg(creator_.map().y_max() / 10));
//}
} // namespace eno
