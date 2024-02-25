#pragma once

#include <QtCore/QList>
#include <QtCore/QObject>
#include <QtCore/QString>

namespace eno {

class RecentFiles : public QObject {
	Q_OBJECT

public:
	RecentFiles(QObject* parent = nullptr);
	virtual ~RecentFiles() = default;

	void reset();

	const QList<QString>& datas() const;
	void add(const QString& data);
	void remove(const QString& data);

private:
	QList<QString> _datas;

signals:
	void datasUpdated();
};
} // namespace eno
