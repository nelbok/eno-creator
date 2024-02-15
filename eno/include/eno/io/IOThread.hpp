#pragma once

#include <QtCore/QThread>

namespace eno {
class Project;

class IOThread : public QThread {
	Q_OBJECT

public:
	enum class Type { Save, Load, Unknwon };
	enum class Result { Success, Canceled, Error, Unknwon };

	IOThread(Project* project, const QString& path, Type type);
	virtual ~IOThread() = default;

	Result result() const;

protected:
	virtual void run() override;
	virtual void save();
	virtual void load();

protected:
	Project* _project{ nullptr };
	const QString _path{};
	const Type _type{ Type ::Unknwon };
	Result _result{ Result::Unknwon };
};
} // namespace eno
