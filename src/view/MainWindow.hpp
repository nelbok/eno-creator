#pragma once

#include <QMainWindow>

class QProgressBar;

namespace eno {
class Project;
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
	void updateWindowTitle();
	void showMessage(const QString& message);
	void showProgressBar(bool visible);

private:
	void initMainMenu();
	void initTools();
	void initLayers();
	void initGenerate();
	void initDocks();

	Project* _project{ nullptr };

	// Controllers
	MapAction* _mapAction{ nullptr };
	Shortcuts* _shortcuts{ nullptr };

	// Interface
	GraphicsView* _graphicsView{ nullptr };
	QProgressBar* _progressBar{ nullptr };
};
} // namespace eno
