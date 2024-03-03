#pragma once

#include <QtCore/QString>
#include <QtGui/QColor>
#include <QtGui/QPixmap>

#include <eno/data/Container.hpp>

namespace eno {
class Project;

class Texture : public Item {
	Q_OBJECT
	Q_PROPERTY(QUuid uuid READ uuid NOTIFY uuidUpdated)
	Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameUpdated)
	Q_PROPERTY(QPixmap data READ data WRITE setData NOTIFY dataUpdated)

public:
	Texture(Project* project);
	Texture(const QUuid& uuid, Project* project);
	virtual ~Texture() = default;

	// clang-format off
	const QString& name() const { return _name; }
	void setName(const QString& name);

	const QPixmap& data() const { return _data; }
	void setData(const QPixmap& data);
	// clang-format on

private:
	QString _name{};
	QPixmap _data{};

signals:
	void nameUpdated();
	void dataUpdated();
};
} // namespace eno
