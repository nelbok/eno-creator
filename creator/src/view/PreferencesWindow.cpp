#include "PreferencesWindow.hpp"

#include <QtWidgets/QApplication>
#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTabWidget>

#include "controller/Preferences.hpp"
#include "widgets/common/BoundingBox.hpp"
#include "widgets/common/ColorButton.hpp"
#include "widgets/common/KeyButton.hpp"
#include "widgets/LocationWidget.hpp"

namespace eno {

PreferencesWindow::PreferencesWindow(QWidget* parent)
	: QWidget(parent) {
	setWindowTitle(qApp->applicationName() + " - Preferences");
	setAttribute(Qt::WA_DeleteOnClose);
}

void PreferencesWindow::initUi() {
	setFixedSize(640, 390);
	setWindowModality(Qt::ApplicationModal);

	// Tab widget
	_tabWidget = new QTabWidget(this);
	auto* l = new QHBoxLayout(this);
	l->addWidget(_tabWidget);

	initGeneral();
	initProject();
	initMaterial();
	initKeyBinding();
}

void PreferencesWindow::closeEvent(QCloseEvent* e) {
	// General
	{
		// Locations
		Preferences::setProjectLocation(_project->location());
		Preferences::setGenerateLocation(_generate->location());

		// Tools
		Preferences::setMapDepth(_depth->value());
		Preferences::setMapPenWidth(_penWidth->value());
		Preferences::setMapZoom(_zoom->currentData().value<Preferences::Zoom>());

		// Optimizations
		Preferences::setGenerationOptimized(_opti1->isChecked());
	}

	// Scene
	{
		Preferences::setSceneMin({
			_minX->value(),
			_minY->value(),
		});
		Preferences::setSceneMax({
			_maxX->value(),
			_maxY->value(),
		});
	}

	// Material
	{
		Preferences::setMaterialName(_name->text());
		Preferences::setMaterialDiffuse(_diffuse->color());
	}

	// Key binding
	{
		Preferences::setKeyRemove(_remove->key());
		Preferences::setKeyAdd(_add->key());
		Preferences::setKeyPicker(_picker->key());
		Preferences::setKeyResize(_resize->key());
		Preferences::setKey3DView(_3DView->key());
	}

	QWidget::closeEvent(e);
}

void PreferencesWindow::initGeneral() {
	auto* form = initTab("General", "Locations, tools, optimizations");

	// Locations
	_project = new LocationWidget;
	_generate = new LocationWidget;

	_project->init();
	_generate->init();

	_project->setMaximumWidth(200);
	_generate->setMaximumWidth(200);

	_project->setLocation(Preferences::projectLocation());
	_generate->setLocation(Preferences::generateLocation());

	form->addRow("Default project location:", _project);
	form->addRow("Default generate location:", _generate);

	// Tools
	_depth = new QSpinBox;
	_depth->setRange(Preferences::minDepth, Preferences::maxDepth);

	_penWidth = new QSpinBox;
	_penWidth->setRange(Preferences::minPenWidth, Preferences::maxPenWidth);

	_zoom = new QComboBox;
	_zoom->addItem(Preferences::toString(Preferences::Zoom::x25), QVariant::fromValue(Preferences::Zoom::x25));
	_zoom->addItem(Preferences::toString(Preferences::Zoom::x50), QVariant::fromValue(Preferences::Zoom::x50));
	_zoom->addItem(Preferences::toString(Preferences::Zoom::x100), QVariant::fromValue(Preferences::Zoom::x100));
	_zoom->addItem(Preferences::toString(Preferences::Zoom::x200), QVariant::fromValue(Preferences::Zoom::x200));

	_depth->setMaximumWidth(60);
	_penWidth->setMaximumWidth(60);
	_zoom->setMaximumWidth(60);

	_depth->setValue(Preferences::mapDepth());
	_penWidth->setValue(Preferences::mapPenWidth());
	_zoom->setCurrentIndex(_zoom->findData(QVariant::fromValue(Preferences::mapZoom())));

	form->addRow("Default depth:", _depth);
	form->addRow("Default pen width:", _penWidth);
	form->addRow("Default zoom:", _zoom);

	// Optimizations
	_opti1 = new QCheckBox;

	_opti1->setMaximumWidth(200);

	_opti1->setChecked(Preferences::generationOptimized());

	form->addRow("Wavefront optimization:", _opti1);
}

void PreferencesWindow::initProject() {
	auto* form = initTab("Project", "Default project");

	_minX = new BoundingBox(this);
	_minY = new BoundingBox(this);
	_maxX = new BoundingBox(this);
	_maxY = new BoundingBox(this);

	_minX->init(false);
	_minY->init(false);
	_maxX->init(true);
	_maxY->init(true);

	const auto& min = Preferences::sceneMin();
	_minX->setValue(min.x());
	_minY->setValue(min.y());
	const auto& max = Preferences::sceneMax();
	_maxX->setValue(max.x());
	_maxY->setValue(max.y());

	form->addRow("Min X:", _minX);
	form->addRow("Min Y:", _minY);
	form->addRow("Max X:", _maxX);
	form->addRow("Max Y:", _maxY);
}

void PreferencesWindow::initMaterial() {
	auto* form = initTab("Material", "Default material");

	_name = new QLineEdit;
	_diffuse = new ColorButton;

	_name->setMaximumWidth(200);
	_diffuse->setMaximumWidth(200);

	_name->setText(Preferences::materialName());
	_diffuse->setColor(Preferences::materialDiffuse());

	form->addRow("Name:", _name);
	form->addRow("Diffuse:", _diffuse);
}

void PreferencesWindow::initKeyBinding() {
	auto* form = initTab("Key Binding", "Key configuration");

	_remove = new KeyButton;
	_add = new KeyButton;
	_picker = new KeyButton;
	_resize = new KeyButton;
	_3DView = new KeyButton;

	_remove->setKey(Preferences::keyRemove());
	_add->setKey(Preferences::keyAdd());
	_picker->setKey(Preferences::keyPicker());
	_resize->setKey(Preferences::keyResize());
	_3DView->setKey(Preferences::key3DView());

	form->addRow("Remove:", _remove);
	form->addRow("Add:", _add);
	form->addRow("Picker:", _picker);
	form->addRow("Resize:", _resize);
	form->addRow("3D View:", _3DView);
}

QFormLayout* PreferencesWindow::initTab(const QString& name, const QString& description) {
	auto* w = new QWidget;
	auto* l = new QVBoxLayout(w);
	_tabWidget->addTab(w, name);

	// Title
	auto* title = new QLabel(description);
	title->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
	title->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);
	l->addWidget(title);

	// Form
	auto* wForm = new QWidget;
	wForm->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Expanding);
	l->addWidget(wForm);

	auto* form = new QFormLayout(wForm);
	form->setFormAlignment(Qt::AlignHCenter | Qt::AlignTop);
	form->setLabelAlignment(Qt::AlignRight);
	form->setVerticalSpacing(20);

	return form;
}

} // namespace eno
