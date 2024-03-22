#pragma once

#include <QtWidgets/QWidget>

class QCheckBox;
class QComboBox;
class QFormLayout;
class QLineEdit;
class QSpinBox;
class QTabWidget;

namespace eno {
class BoundingBox;
class ColorButton;
class KeyButton;
class LocationWidget;

class PreferencesWindow : public QWidget {
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
	BoundingBox* _minX{ nullptr };
	BoundingBox* _minY{ nullptr };
	BoundingBox* _maxX{ nullptr };
	BoundingBox* _maxY{ nullptr };

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
