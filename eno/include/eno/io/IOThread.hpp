#pragma once

#include <QtCore/QObject>

namespace eno {
class Project;

class IOThread : public QObject {
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
	Q_INVOKABLE void start();
	Q_INVOKABLE Result result() const;
	Q_INVOKABLE void requestInterruption();

protected:
	bool isInterruptionRequested();
	virtual void save();
	virtual void load();

protected:
	Project* _project{ nullptr };
	QString _filePath{};
	Type _type{ Type::NoType };
	Result _result{ Result::NoResult };

private:
	class Impl;
	Impl* _impl{ nullptr };

signals:
	void finished();
};
} // namespace eno
