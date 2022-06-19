#pragma once

#include <QThread>

namespace eno {
class Project;

class IOThread : public QThread {
	Q_OBJECT

public:
	enum class Type { Save, Load };

	IOThread(Project* project, const QString& path, Type type);
	virtual ~IOThread() = default;

	bool result() const;

protected:
	virtual void run() override;
	virtual bool save();
	virtual bool load();

protected:
	Project* _project{ nullptr };
	const QString _path{};
	const Type _type{ Type ::Save };
	bool _result{ false };
};
} // namespace eno
