#pragma once

#include <QDockWidget>

class QLabel;
class QLineEdit;
class QListWidget;
class QPushButton;

namespace eno {
class MapAction;
class Material;
class Materials;

class MaterialsDockWidget : public QDockWidget {
	Q_OBJECT

public:
	MaterialsDockWidget(QWidget* parent = 0);
	virtual ~MaterialsDockWidget() = default;

	void init(Materials* materials, MapAction* mapAction);

	Material* currentMaterialSelected() const;

private slots:
	void resetList();
	void updateForm();
	void chooseDiffuse();

private:
	Materials* _materials{ nullptr };
	MapAction* _mapAction{ nullptr };

	QListWidget* _list{ nullptr };
	QLineEdit* _name{ nullptr };
	QPushButton* _diffuse{ nullptr };
	QLabel* _refCount{ nullptr };

signals:
	void showMessage(const QString& message);
};
} // namespace eno
