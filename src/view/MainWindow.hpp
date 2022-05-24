#pragma once

#include <QMainWindow>

class QSpinBox;
class QComboBox;

namespace eno {
class GraphicsView;
class MapAction;
class Shortcuts;

class MainWindow : public QMainWindow {
	Q_OBJECT

public:
	MainWindow(QWidget* parent = 0);
	virtual ~MainWindow() = default;

	void initUi();

protected:
	virtual void closeEvent(QCloseEvent* event);

private slots:
	void reset();
	void updateWindowTitle();
	void showMessage(const QString& message);

private:
	void initMainMenu();
	void initTools();
	void initLayers();

	// Controllers
	MapAction* _mapAction{ nullptr };
	Shortcuts* _shortcuts{ nullptr };

	// Interface
	GraphicsView* _graphicsView{ nullptr };
	QSpinBox* _depthSpinBox{ nullptr };
	QSpinBox* _penWidthSpinBox{ nullptr };
	QComboBox* _zoomComboBox{ nullptr };
};
} // namespace eno
