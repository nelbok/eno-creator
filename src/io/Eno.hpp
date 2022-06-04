#pragma once

class QString;

namespace eno {
class MapAction;
class Project;

class Eno {
public:
	static constexpr auto fileType = "ENO file (*.eno)";
	static constexpr auto fileVersion = 1u;

	Eno(MapAction* mapAction);
	virtual ~Eno() = default;

	bool save(const QString& path);
	bool load(const QString& path);

private:
	Project* _project{ nullptr };
};
} // namespace eno
