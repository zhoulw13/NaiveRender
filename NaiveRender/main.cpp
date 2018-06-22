#define MAIN
#include "variables.h"

#include "NaiveRender.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	NaiveRender w;
	w.show();
	return a.exec();
}
