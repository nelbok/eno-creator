#include "ProjectDockWidget.hpp"

#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLineEdit>

#include <eno/data/Project.hpp>

#include "controller/command/ProjectCommand.hpp"
#include "controller/Core.hpp"

namespace eno {
ProjectDockWidget::ProjectDockWidget(QWidget* parent)
	: QDockWidget(parent) {}

void ProjectDockWidget::init(Core* core) {
	assert(core);
	_core = core;

	setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	setWindowTitle("Project");

	auto* w = new QWidget(this);
	auto* l = new QVBoxLayout(w);

	_name = new QLineEdit(w);

	auto* form = new QFormLayout;
	form->addRow("Name:", _name);
	l->addLayout(form);

	setWidget(w);

	connect(_name, &QLineEdit::returnPressed, this, [this]() {
		ProjectCommand::setName(_core->commands(), _core->project(), _name->text());
		emit showMessage(QString("Project's name changed to %1").arg(_name->text()));
	});

	connect(_core->project(), &Project::nameUpdated, this, &ProjectDockWidget::updateForm);
	updateForm();
}

void ProjectDockWidget::updateForm() {
	assert(_core);
	_name->setText(_core->project()->name());
}

} // namespace eno
