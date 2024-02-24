#include "ProjectCommand.hpp"

#include <QtCore/QString>

#include <eno/data/Project.hpp>

#include "controller/Commands.hpp"
#include "ValueCommand.hpp"

namespace eno {

void ProjectCommand::setName(Commands* c, Project* p, const QString& newValue) {
	addValueCommand(c, p, &Project::setName, &Project::name, newValue);
}

} // namespace eno
