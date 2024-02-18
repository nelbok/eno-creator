#include <QtGui/QGuiApplication>
#include <QtGui/QIcon>
#include <QtQml/QQmlApplicationEngine>

#include <eno/data/Project.hpp>
#include <eno/io/Eno.hpp>

#include "Config.hpp"

int main(int argc, char* argv[]) {
	QGuiApplication a(argc, argv);
	a.setOrganizationName(eno::Config::organization);
	a.setApplicationName(eno::Config::viewer);
	a.setApplicationVersion(eno::Config::version);
	a.setWindowIcon(QIcon(":/logo/logo.png"));

	QScopedPointer<eno::Project> project(new eno::Project);
	project->init();

	//auto* loader = new eno::Eno(project.get(), "path_to_file", eno::IOThread::Type::Load);
	//loader->start();
	//loader->wait();
	//loader->deleteLater();

	QQmlApplicationEngine engine;

	qmlRegisterSingletonInstance("Eno", 1, 0, "MyProject", project.get());

	const QUrl url(u"qrc:/Viewer/qml/Main.qml"_qs);
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
