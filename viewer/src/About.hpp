#pragma once

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QUrl>

namespace eno {
struct About : public QObject {
	Q_OBJECT
	Q_PROPERTY(QString text READ text CONSTANT)
	Q_PROPERTY(QUrl logo READ logo CONSTANT)

	QString text();
	QUrl logo();
};
} // namespace eno
