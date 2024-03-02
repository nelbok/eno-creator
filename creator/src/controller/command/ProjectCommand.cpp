#include "ProjectCommand.hpp"

#include <QtCore/QOverload>
#include <QtCore/QString>

#include <eno/data/Project.hpp>

#include "controller/Commands.hpp"
#include "ContainerCommand.hpp"
#include "ValueCommand.hpp"

namespace eno {

void ProjectCommand::setName(Commands* c, Project* p, const QString& newValue) {
	addValueCommand(c, p, &Project::setName, &Project::name, newValue);
}

void ProjectCommand::addTag(Commands* c, Project* p, const QStringList& t) {
	c->add(new ContainerCommand(p, qOverload<const QStringList&>(&Project::add), qOverload<const QStringList&>(&Project::remove), t));
}

void ProjectCommand::removeTag(Commands* c, Project* p, const QStringList& t) {
	c->add(new ContainerCommand(p, qOverload<const QStringList&>(&Project::remove), qOverload<const QStringList&>(&Project::add), t));
}

} // namespace eno
