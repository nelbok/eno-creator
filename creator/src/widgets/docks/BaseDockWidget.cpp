#include "BaseDockWidget.hpp"

#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>

namespace eno {
BaseDockWidget::BaseDockWidget(QWidget* parent)
	: QDockWidget(parent) {}

void BaseDockWidget::init(Core* core) {
	assert(core);
	_core = core;

	setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	setWindowTitle(dockName());

	auto* w = new QWidget(this);
	_layout = new QVBoxLayout(w);

	initContent();

	setWidget(w);
}

void BaseDockWidget::updateListDatas() {
	assert(_list);

	QVariant current = currentListData();
	QListWidgetItem* currentItem = nullptr;

	_list->blockSignals(true);
	_list->clear();
	for (const auto& pair : datas()) {
		auto* item = new QListWidgetItem;
		item->setText(pair.first);
		item->setData(Qt::UserRole, pair.second);
		item->setFlags(item->flags() | Qt::ItemIsEditable);
		_list->addItem(item);

		if (current == pair.second) {
			currentItem = item;
		}
	}
	_list->blockSignals(false);
	if (currentItem) {
		_list->setCurrentItem(currentItem);
	} else if (_list->count()) {
		_list->setCurrentItem(_list->item(0));
	}
	currentListDataChanged();
}

void BaseDockWidget::initList(const QString& addName, const QString& removeName) {
	auto* w = _layout->parentWidget();

	auto* buttonAdd = new QPushButton(addName, w);
	auto* buttonRemove = new QPushButton(removeName, w);

	auto* tb = new QHBoxLayout;
	tb->addWidget(buttonAdd);
	tb->addWidget(buttonRemove);

	connect(buttonAdd, &QPushButton::clicked, this, &BaseDockWidget::addClicked);
	connect(buttonRemove, &QPushButton::clicked, this, &BaseDockWidget::removeClicked);

	_list = new QListWidget(w);
	_list->setSortingEnabled(true);
	connect(_list, &QListWidget::itemSelectionChanged, this, &BaseDockWidget::currentListDataChanged);
	connect(_list, &QListWidget::itemChanged, this, &BaseDockWidget::currentListDataEdited);
	updateListDatas();

	_layout->addLayout(tb);
	_layout->addWidget(_list);
}

void BaseDockWidget::setCurrentListData(QVariant current) {
	assert(_list);
	int count = _list->count();
	for (int i = 0; i < count; ++i) {
		auto* item = _list->item(i);
		if (item->data(Qt::UserRole) == current) {
			_list->setCurrentItem(item);
			break;
		}
	}
}

QString BaseDockWidget::currentListName() const {
	assert(_list);
	if (auto* current = _list->currentItem()) {
		return current->text();
	}
	return {};
}

QVariant BaseDockWidget::currentListData() const {
	assert(_list);
	if (auto* current = _list->currentItem()) {
		return current->data(Qt::UserRole);
	}
	return {};
}
} // namespace eno
