#include "ProjectDockWidget.hpp"

#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>

#include <eno/data/Project.hpp>

#include "controller/command/ProjectCommand.hpp"
#include "controller/Commands.hpp"
#include "controller/Core.hpp"

namespace eno {
ProjectDockWidget::ProjectDockWidget(QWidget* parent)
	: BaseDockWidget(parent) {}

QString ProjectDockWidget::dockName() const {
	return "Project";
}

void ProjectDockWidget::initContent() {
	initInfos();
	initList("Add tag", "Remove tag");
	initStats();

	connect(_core->project(), &Project::tagsUpdated, this, &BaseDockWidget::updateListDatas);
}

void ProjectDockWidget::addClicked() {
	ProjectCommand::addTag(_core->commands(), _core->project(), { "My tag" });
	emit showMessage("Tag added");
}

void ProjectDockWidget::removeClicked() {
	const auto& current = currentListData();
	if (current.isValid()) {
		ProjectCommand::removeTag(_core->commands(), _core->project(), { current.toString() });
		emit showMessage(QString("Tag %1 removed").arg(current.toString()));
	}
}

void ProjectDockWidget::currentListDataChanged() {
	// Nothing to do
}

void ProjectDockWidget::currentListDataEdited() {
	const auto& newName = currentListName();
	const auto& oldName = currentListData().toString();
	auto* c = _core->commands();
	auto* p = _core->project();
	c->beginList();
	ProjectCommand::removeTag(c, p, { oldName });
	ProjectCommand::addTag(c, p, { newName });
	c->endList();
	emit showMessage(QString("Tag's name changed to %1").arg(newName));
}

QList<QPair<QString, QVariant>> ProjectDockWidget::datas() const {
	QList<QPair<QString, QVariant>> datas;
	for (const auto& tag : _core->project()->tags()) {
		datas.append({ tag, tag });
	}
	return datas;
}

void ProjectDockWidget::initInfos() {
	auto* w = _layout->parentWidget();

	auto* name = new QLineEdit(w);
	connect(name, &QLineEdit::returnPressed, this, [this, name]() {
		ProjectCommand::setName(_core->commands(), _core->project(), name->text());
		emit showMessage(QString("Project's name changed to %1").arg(name->text()));
	});
	connect(_core->project(), &Project::nameUpdated, this, [this, name]() {
		name->setText(_core->project()->name());
	});
	name->setText(_core->project()->name());

	auto* form = new QFormLayout;
	form->addRow("Name:", name);
	_layout->addLayout(form);
}

void ProjectDockWidget::initStats() {
	auto* w = _layout->parentWidget();

	auto* objects = new QLabel(w);
	auto* materials = new QLabel(w);

	auto lambdaObjects = [this, objects](const auto& items) {
		objects->setText(QString("Total cube(s) in the project: %1").arg(items.count()));
	};
	connect(_core->project()->scene(), &Scene::objectsUpdated, this, lambdaObjects);
	lambdaObjects(_core->project()->scene()->objects());

	auto lambdaMaterials = [this, materials](const auto& items) {
		materials->setText(QString("Total material(s) in the project: %1").arg(items.count()));
	};
	connect(_core->project(), &Project::materialsUpdated, this, lambdaMaterials);
	lambdaMaterials(_core->project()->materials());

	_layout->addWidget(objects);
	_layout->addWidget(materials);
}

} // namespace eno
