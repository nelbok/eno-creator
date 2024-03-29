#pragma once

#include "BaseDockWidget.hpp"

class QCheckBox;
class QLabel;
class QLineEdit;

namespace eno {
class PixmapButton;
class Texture;

class TexturesDockWidget : public BaseDockWidget {
	Q_OBJECT

public:
	TexturesDockWidget(QWidget* parent = 0);
	virtual ~TexturesDockWidget() = default;

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

	Texture* _current{ nullptr };
	QWidget* _form{ nullptr };
	QLineEdit* _name{ nullptr };
	PixmapButton* _data{ nullptr };
	QCheckBox* _invertX{ nullptr };
	QCheckBox* _invertY{ nullptr };
	QLabel* _refCount{ nullptr };
};
} // namespace eno
