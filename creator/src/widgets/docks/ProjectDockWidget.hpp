#pragma once

#include "BaseDockWidget.hpp"

namespace eno {
class ProjectDockWidget : public BaseDockWidget {
	Q_OBJECT

public:
	ProjectDockWidget(QWidget* parent = 0);
	virtual ~ProjectDockWidget() = default;

protected:
	virtual QString dockName() const override;
	virtual void initContent() override;

	virtual void addClicked() override;
	virtual void removeClicked() override;
	virtual void currentListDataChanged() override;
	virtual void currentListDataEdited() override;
	virtual QList<QPair<QString, QVariant>> datas() const override;

private:
	void initInfos();
	void initStats();
};
} // namespace eno
