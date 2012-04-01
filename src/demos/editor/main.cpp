#include "mainwindow.h"

#include <QApplication>
#include <QPushButton>

int main(int argc, char** argv)
{
	QApplication app(argc, argv);
	app.setStyleSheet("QTextEdit { background-color: #303030; } \n QDockWidget {border-width: 0px; padding: 0px; margin:0px;} \n QDockWidget::title	 { foreground: #B0B0B0; text-align: right; background: #404040; }");

	MainWindow mainWindow;
	mainWindow.showMaximized();
	return app.exec();
}
