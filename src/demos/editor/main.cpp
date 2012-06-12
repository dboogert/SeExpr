#include "mainwindow.h"

#include <QApplication>
#include <QPushButton>
#include <ri.h>

int main(int argc, char** argv)
{
	RiBegin("test.rib");
	RiFormat(1024,768,1);
	RiPixelSamples(3,3);

	RiWorldBegin();
	RiSphere(1,-1,1,360);
	RiWorldEnd();
	RiEnd();


	QApplication app(argc, argv);
	app.setStyleSheet(" QListWidget { color: #B0B0B0; background-color: #505050; }\n QTextEdit { color: #B0B0B0; background-color: #303030; } \n QDockWidget { color: #B0B0B0; border-width: 0px; padding: 0px; margin:0px;} \n QDockWidget::title	 { color: #B0B0B0; text-align: right; background: #404040; }");

	MainWindow mainWindow;
	mainWindow.showMaximized();
	return app.exec();
}
