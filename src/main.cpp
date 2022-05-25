#include <QApplication>

#include "view/MainWindow.hpp"

int main(int argc, char* argv[]) {
	QApplication a(argc, argv);
	a.setApplicationName("Eno Creator");

	eno::MainWindow w;
	w.initUi();
	w.show();

	return a.exec();
}
