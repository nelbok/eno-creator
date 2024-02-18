#pragma once

#include <QtWidgets/QDockWidget>

class QLabel;
class QLineEdit;
class QListWidget;

namespace eno {
class ColorButton;
class MapAction;
class Material;
class Project;

class MaterialsDockWidget : public QDockWidget {
	Q_OBJECT

public:
	MaterialsDockWidget(QWidget* parent = 0);
	virtual ~MaterialsDockWidget() = default;

	void init(MapAction* mapAction);

private slots:
	void resetList();
	void updateForm();

private:
	MapAction* _mapAction{ nullptr };
	Material* _current{ nullptr };

	QListWidget* _list{ nullptr };
	QLineEdit* _name{ nullptr };
	ColorButton* _diffuse{ nullptr };
	QLabel* _refCount{ nullptr };

signals:
	void showMessage(const QString& message);
};
} // namespace eno
