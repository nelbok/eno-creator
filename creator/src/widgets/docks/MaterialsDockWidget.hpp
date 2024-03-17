#pragma once

#include "BaseDockWidget.hpp"

class QComboBox;
class QLabel;
class QLineEdit;

namespace eno {
class ColorButton;
class Material;
class Texture;

class MaterialsDockWidget : public BaseDockWidget {
	Q_OBJECT

public:
	MaterialsDockWidget(QWidget* parent = 0);
	virtual ~MaterialsDockWidget() = default;

protected:
	virtual QString dockName() const override;
	virtual void initContent() override;

	virtual void addClicked() override;
	virtual void removeClicked() override;
	virtual void currentListDataChanged() override;
	virtual void currentListDataEdited() override;
	virtual QList<QPair<QString, QVariant>> datas() const override;

private slots:
	void updateTextures(const QList<Texture*>& textures);
	void updateTextureName();

private:
	void initForm();
	void updateForm();

	Material* _current{ nullptr };
	QWidget* _form{ nullptr };
	QLineEdit* _name{ nullptr };
	ColorButton* _diffuse{ nullptr };
	QComboBox* _diffuseMap{ nullptr };
	QLabel* _refCount{ nullptr };
};
} // namespace eno
