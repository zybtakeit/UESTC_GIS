#include "UESTC_GIS.h"
#include <QtWidgets/QApplication>
#include <qgsapplication.h>

int main(int argc, char *argv[])
{
	//QApplication a(argc, argv);
	QgsApplication a(argc, argv, true);
	QgsApplication::initQgis();

	UESTC_GIS w;
	w.show();
	return a.exec();
}
