#pragma once

#include "BaseDockWidget.hpp"

class QLabel;
class QLineEdit;

namespace eno {
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
	QLineEdit* _name{ nullptr };
	QLabel* _refCount{ nullptr };
};
} // namespace eno
