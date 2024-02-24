#pragma once

#include <QtCore/QThread>

namespace eno {
class Project;

class IOThread : public QThread {
	Q_OBJECT

public:
	enum class Type { Save, Load, NoType };
	enum class Result { Success, Canceled, Error, NoResult };

	Q_ENUM(Type)
	Q_ENUM(Result)

	IOThread(QObject* parent = nullptr);
	virtual ~IOThread() = default;

	Q_INVOKABLE void init(Project* project, Type type, const QUrl& url);
	Q_INVOKABLE void init(Project* project, Type type, const QString& filePath);
	Q_INVOKABLE Result result() const;

protected:
	virtual void run() override;
	virtual void save();
	virtual void load();

protected:
	Project* _project{ nullptr };
	QString _filePath{};
	Type _type{ Type::NoType };
	Result _result{ Result::NoResult };
};
} // namespace eno
