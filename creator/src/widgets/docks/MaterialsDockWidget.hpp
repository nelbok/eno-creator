#pragma once

#include "BaseDockWidget.hpp"

class QComboBox;
class QLabel;
class QLineEdit;

namespace eno {
class ColorButton;
class Material;
class PercentSlider;
class Texture;
class TextureBox;

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

private:
	void initForm();
	void updateForm();

	Material* _current{ nullptr };
	QWidget* _form{ nullptr };
	QLineEdit* _name{ nullptr };
	ColorButton* _diffuse{ nullptr };
	TextureBox* _diffuseMap{ nullptr };
	PercentSlider* _opacity{ nullptr };
	TextureBox* _opacityMap{ nullptr };
	QLabel* _refCount{ nullptr };
};
} // namespace eno
