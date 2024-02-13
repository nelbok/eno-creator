#pragma once

#include <QMainWindow>

class QCheckBox;
class QComboBox;
class QFormLayout;
class QLineEdit;
class QSpinBox;

namespace eno {
class ColorButton;
class KeyButton;
class LocationWidget;
class SpinBox;

class PreferencesWindow : public QMainWindow {
	Q_OBJECT

public:
	PreferencesWindow(QWidget* parent = 0);
	virtual ~PreferencesWindow() = default;

	void initUi();

protected:
	virtual void closeEvent(QCloseEvent* event);

private:
	void initGeneral();
	void initProject();
	void initMaterial();
	void initKeyBinding();

	QFormLayout* initTab(const QString& name, const QString& description);

	QTabWidget* _tabWidget{ nullptr };

	// Locations
	LocationWidget* _project{ nullptr };
	LocationWidget* _generate{ nullptr };

	// Tools
	QSpinBox* _depth{ nullptr };
	QSpinBox* _penWidth{ nullptr };
	QComboBox* _zoom{ nullptr };

	// Optimizations
	QCheckBox* _opti1{ nullptr };

	// Scene
	SpinBox* _minX{ nullptr };
	SpinBox* _minY{ nullptr };
	SpinBox* _maxX{ nullptr };
	SpinBox* _maxY{ nullptr };

	// Material
	QLineEdit* _name{ nullptr };
	ColorButton* _diffuse{ nullptr };

	// Key binding
	KeyButton* _remove{ nullptr };
	KeyButton* _add{ nullptr };
	KeyButton* _picker{ nullptr };
	KeyButton* _resize{ nullptr };
	KeyButton* _3DView{ nullptr };
};
} // namespace eno
