#pragma once

class QString;

namespace eno {
class Data;
class MapAction;

class Eno {
public:
	static constexpr auto fileType = "ENO file (*.eno)";

	Eno(MapAction* mapAction);
	virtual ~Eno() = default;

	bool save(const QString& path);
	bool load(const QString& path);

private:
	Data* _data{ nullptr };
};
} // namespace eno
