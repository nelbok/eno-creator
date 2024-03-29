#include <QtGui/QGuiApplication>
#include <QtGui/QIcon>
#include <QtQml/QQmlApplicationEngine>
#include <QtQml/QQmlExtensionPlugin>

#include <eno/data/Project.hpp>
#include <eno/engine/CuboidGeometry.hpp>
#include <eno/engine/CuboidTexture.hpp>
#include <eno/io/Eno.hpp>

#include "About.hpp"
#include "Intro.hpp"

#include "Config.hpp"

Q_IMPORT_QML_PLUGIN(enginePlugin)

int main(int argc, char* argv[]) {
	QGuiApplication a(argc, argv);
	a.setOrganizationName(eno::Config::organization);
	a.setApplicationName(eno::Config::viewer);
	a.setApplicationVersion(eno::Config::version);
	a.setWindowIcon(QIcon(":/logo/logo.png"));

	QScopedPointer<eno::About> about(new eno::About);
	QScopedPointer<eno::Project> project(new eno::Project);
	QScopedPointer<eno::Intro> intro(new eno::Intro);

	project->init();
	intro->initIntro();

	QQmlApplicationEngine engine;

	qmlRegisterSingletonInstance("Eno", 1, 0, "MyAbout", about.get());
	qmlRegisterSingletonInstance("Eno", 1, 0, "MyIntro", intro.get());
	qmlRegisterSingletonInstance("Eno", 1, 0, "MyProject", project.get());
	qmlRegisterType<eno::CuboidGeometry>("Eno", 1, 0, "MyCuboidGeometry");
	qmlRegisterType<eno::CuboidTexture>("Eno", 1, 0, "MyCuboidTexture");
	qmlRegisterType<eno::Eno>("Eno", 1, 0, "MyLoader");

	const QUrl url("qrc:/viewer/qml/Main.qml");
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
