#pragma once

#include <QtWidgets/QMainWindow>

class QProgressDialog;
class QThread;

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
	void showProgressDialog(bool visible, QThread* thread = nullptr);

private:
	void initMainMenu();
	void initEdit();
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
	QProgressDialog* _progressDialog{ nullptr };
};
} // namespace eno
