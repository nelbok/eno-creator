#include <QtGui/QGuiApplication>
#include <QtGui/QIcon>
#include <QtQml/QQmlApplicationEngine>

#include "Config.hpp"

int main(int argc, char* argv[]) {
	QGuiApplication a(argc, argv);
	a.setOrganizationName(eno::Config::organization);
	a.setApplicationName(eno::Config::creator);
	a.setApplicationVersion(eno::Config::version);
	a.setWindowIcon(QIcon(":/logo/logo.png"));

	QQmlApplicationEngine engine;
	const QUrl url(u"qrc:/EnoViewer/qml/Main.qml"_qs);
	QObject::connect(
		&engine, &QQmlApplicationEngine::objectCreated, &a,
		[url](QObject* obj, const QUrl& objUrl) {
			if (!obj && url == objUrl)
				QCoreApplication::exit(-1);
		},
		Qt::QueuedConnection);
	engine.load(url);

	return a.exec();
}
