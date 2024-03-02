#pragma once

#include <QtCore/QList>
#include <QtCore/QPair>
#include <QtCore/QString>
#include <QtCore/QVariant>
#include <QtWidgets/QDockWidget>

class QListWidget;
class QVBoxLayout;

namespace eno {
class Core;

class BaseDockWidget : public QDockWidget {
	Q_OBJECT

public:
	BaseDockWidget(QWidget* parent = 0);
	virtual ~BaseDockWidget() = default;

	void init(Core* core);

public slots:
	// List
	void updateListDatas() const;

protected slots:
	// List
	// clang-format off
	virtual void addClicked() { assert(false); }
	virtual void removeClicked() { assert(false); }
	virtual void currentListDataChanged() { assert(false); }
	virtual void currentListDataEdited() { assert(false); }
	// clang-format on

protected:
	// Main
	virtual QString dockName() const = 0;
	virtual void initContent() = 0;

	// List
	void initList(const QString& addName, const QString& removeName);
	QString currentListName() const;
	QVariant currentListData() const;

	// clang-format off
	virtual QList<QPair<QString, QVariant>> datas() const { assert(false); return {}; }
	// clang-format on

	Core* _core{ nullptr };
	QVBoxLayout* _layout{ nullptr };

private:
	QListWidget* _list{ nullptr };

signals:
	void showMessage(const QString& message);
};
} // namespace eno
