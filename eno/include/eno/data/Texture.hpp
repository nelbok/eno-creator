#pragma once

#include <QtCore/QString>
#include <QtCore/QSize>
#include <QtCore/QByteArray>
#include <QtGui/QImage>
#include <QtGui/QPixmap>

#include <eno/data/Container.hpp>

namespace eno {
class Project;

class Texture : public Item {
	Q_OBJECT
	Q_PROPERTY(QUuid uuid READ uuid NOTIFY uuidUpdated)
	Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameUpdated)
	Q_PROPERTY(QSize size READ size NOTIFY sizeUpdated)
	Q_PROPERTY(QByteArray data READ data NOTIFY dataUpdated)

public:
	Texture(Project* project);
	Texture(const QUuid& uuid, Project* project);
	virtual ~Texture() = default;

	// clang-format off
	const QString& name() const { return _name; }
	void setName(const QString& name);

	const QSize& size() const { return _size; }
	const QByteArray& data() const { return _data; }
	// clang-format on

	QPixmap pixmap() const;
	QImage image() const;

	// Data should be in Format RGBA8888
	void set(const QSize& size, const QByteArray& data);
	void set(const QImage& image);
	void set(const QPixmap& pixmap);

private:
	QString _name{};
	QSize _size{};
	QByteArray _data{};

signals:
	void nameUpdated();
	void sizeUpdated();
	void dataUpdated();
};
} // namespace eno
