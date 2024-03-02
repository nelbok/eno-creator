#pragma once

#include <QStringList>

class QString;

namespace eno {
class Commands;
class Project;

struct ProjectCommand {
	static void setName(Commands* commands, Project* project, const QString& name);
	static void addTag(Commands* commands, Project* project, const QStringList& tag);
	static void removeTag(Commands* commands, Project* project, const QStringList& tag);
};
} // namespace eno
