#include <QApplication>

#include "Config.hpp"

#include "view/MainWindow.hpp"

int main(int argc, char* argv[]) {
	QApplication a(argc, argv);
	a.setOrganizationName(eno::Config::organization);
	a.setApplicationName(eno::Config::name);
	a.setApplicationVersion(eno::Config::version);

	eno::MainWindow w;
	w.initUi();
	w.show();

	return a.exec();
}
