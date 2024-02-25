#include "About.hpp"

#include <QtGui/QGuiApplication>

#include "Config.hpp"

namespace eno {

QString About::text() {
	QString text = "<strong>About " + QString(Config::viewer) + " - " + QString(Config::version) + "</strong><br/>";
	text += "<br/>";
	text += "3D map viewer.<br/>";
	text += "<br/>";
	text += QString(Config::viewer) + " is an easy application to view your ENO project on smartphone or else.<br/>";
	text += "<br/>";
	text += QString(Config::viewer) + " is licensed under GNU General Public License v3.0.<br/>";
	text += "<br/>";
	text += QString(Config::copyright);
	return text;
}

QUrl About::logo() {
	return { "qrc:/logo/logo.png" };
}

} // namespace eno
