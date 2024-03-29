#pragma once

#include <QtCore/QObject>

class QAction;
class QMenu;

namespace eno {
class Core;
class IOThread;
class Shortcuts : public QObject {
	Q_OBJECT

public:
	Shortcuts(Core* core, QObject* parent = nullptr);
	virtual ~Shortcuts() = default;

	void initActions();
	void resetActions();

public:
	// clang-format off
	// File
	QAction* newAction() const { return _newAction; }
	QAction* openAction() const { return _openAction; }
	QAction* saveAction() const { return _saveAction; }
	QAction* saveAsAction() const { return _saveAsAction; }
	QMenu* recentFilesMenu() const { return _recentFilesMenu; }
	QAction* preferenceAction() const { return _preferenceAction; }
	QAction* quitAction() const { return _quitAction; }

	// Edit
	QAction* undoAction() const { return _undoAction; }
	QAction* redoAction() const { return _redoAction; }

	// Tools
	QAction* removeAction() const { return _removeAction; }
	QAction* addAction() const { return _addAction; }
	QAction* pickerAction() const { return _pickerAction; }
	QAction* resizeAction() const { return _resizeAction; }

	// Generate
	QAction* generateOBJAction() const { return _generateOBJAction; }
	QAction* generate3DAction() const { return _generate3DAction; }

	// Help
	QAction* githubAction() const { return _githubAction; }
	QAction* twitterAction() const { return _twitterAction; }
	QAction* discordAction() const { return _discordAction; }
	QAction* aboutAction() const { return _aboutAction; }
	QAction* aboutQtAction() const { return _aboutQtAction; }
	// clang-format on

private:
	void initFile();
	void initEdit();
	void initTools();
	void initGenerate();
	void initHelp();

	bool needToSave();
	void open(const QString& path);
	void save(bool newPathRequested);

	Core* _core{ nullptr };

	// Tools action
	QAction* _newAction{ nullptr };
	QAction* _openAction{ nullptr };
	QAction* _saveAction{ nullptr };
	QAction* _saveAsAction{ nullptr };
	QMenu* _recentFilesMenu{ nullptr };
	QAction* _preferenceAction{ nullptr };
	QAction* _quitAction{ nullptr };

	// Edit action
	QAction* _undoAction{ nullptr };
	QAction* _redoAction{ nullptr };

	// Tools action
	QAction* _removeAction{ nullptr };
	QAction* _addAction{ nullptr };
	QAction* _pickerAction{ nullptr };
	QAction* _resizeAction{ nullptr };

	// Generate action
	QAction* _generateOBJAction{ nullptr };
	QAction* _generate3DAction{ nullptr };

	// Others action
	QAction* _githubAction{ nullptr };
	QAction* _twitterAction{ nullptr };
	QAction* _discordAction{ nullptr };
	QAction* _aboutAction{ nullptr };
	QAction* _aboutQtAction{ nullptr };

signals:
	void showMessage(const QString& message);
	void showProgressDialog(bool visible, IOThread* thread = nullptr);
};
} // namespace eno
