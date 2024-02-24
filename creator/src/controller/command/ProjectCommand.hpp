#pragma once

class QString;

namespace eno {
class Commands;
class Project;

struct ProjectCommand {
	static void setName(Commands* commands, Project* project, const QString& name);
};
} // namespace eno
