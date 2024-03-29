#pragma once

#include <QtWidgets/QMainWindow>

class QProgressDialog;

namespace eno {
class Core;
class GraphicsView;
class IOThread;
class Project;
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
	void showProgressDialog(bool visible, IOThread* thread = nullptr);

private:
	void initFile();
	void initEdit();
	void initTools();
	void initLayers();
	void initGenerate();
	void initHelp();
	void initDocks();

	Project* _project{ nullptr };

	// Controllers
	Core* _core{ nullptr };
	Shortcuts* _shortcuts{ nullptr };

	// Interface
	GraphicsView* _graphicsView{ nullptr };
	QProgressDialog* _progressDialog{ nullptr };
};
} // namespace eno
