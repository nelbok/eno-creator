#pragma once

#include <QObject>

class QAction;

namespace eno {
class MapAction;
class Shortcuts : public QObject {
	Q_OBJECT

public:
	Shortcuts(MapAction* mapAction, QObject* parent = nullptr);
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
	QAction* quitAction() const { return _quitAction; }

	// Tools
	QAction* removeAction() const { return _removeAction; }
	QAction* addAction() const { return _addAction; }
	QAction* pickerAction() const { return _pickerAction; }
	QAction* resizeAction() const { return _resizeAction; }
	QAction* colorDialogAction() const { return _colorDialogAction; }

	// Generate
	QAction* generateOBJAction() const { return _generateOBJAction; }
	QAction* generate3DAction() const { return _generate3DAction; }
	// clang-format on

private slots:
	void updateColorDialogIcon();

private:
	void initFile();
	void initTools();
	void initGenerate();

	bool save(bool newPathRequested);

	MapAction* _mapAction{ nullptr };

	// Tools action
	QAction* _newAction{ nullptr };
	QAction* _openAction{ nullptr };
	QAction* _saveAction{ nullptr };
	QAction* _saveAsAction{ nullptr };
	QAction* _quitAction{ nullptr };

	// Tools action
	QAction* _removeAction{ nullptr };
	QAction* _addAction{ nullptr };
	QAction* _pickerAction{ nullptr };
	QAction* _resizeAction{ nullptr };
	QAction* _colorDialogAction{ nullptr };

	// Tools action
	QAction* _generateOBJAction{ nullptr };
	QAction* _generate3DAction{ nullptr };

signals:
	void updated();
	void showMessage(const QString& message);
};
} // namespace eno
