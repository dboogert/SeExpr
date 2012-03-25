#include "mainwindow.h"
#include <QtGui>

#include <iostream>
#include "imager.h"

 MainWindow::MainWindow(QWidget *parent)
     : QMainWindow(parent)
 {
     setupFileMenu();
     setupHelpMenu();
     setupEditor();

     setCentralWidget(m_editor);
     setWindowTitle(tr("Se.Expr Editor"));
 }

 void MainWindow::about()
 {
     QMessageBox::about(this, tr("About Syntax Highlighter"),
                 tr("<p>The <b>Syntax Highlighter</b> example shows how " \
                    "to perform simple syntax highlighting by subclassing " \
                    "the QSyntaxHighlighter class and describing " \
                    "highlighting rules using regular expressions.</p>"));
 }

 void MainWindow::newFile()
 {
     m_editor->clear();
 }

 void MainWindow::openFile(const QString &path)
 {
     QString fileName = path;

     if (fileName.isNull())
         fileName = QFileDialog::getOpenFileName(this,
             tr("Open File"), "", "C++ Files (*.cpp *.h)");

     if (!fileName.isEmpty()) {
         QFile file(fileName);
         if (file.open(QFile::ReadOnly | QFile::Text))
             m_editor->setPlainText(file.readAll());
     }
 }

 void MainWindow::setupEditor()
 {
     QFont font;
     font.setFamily("Courier");
     font.setFixedPitch(true);
     font.setPointSize(12);

     m_editor = new QTextEdit;
     m_editor->setFont(font);

     m_highlighter = new Highlighter(m_editor->document());

     QFile file("../mainwindow.h");
     if (file.open(QFile::ReadOnly | QFile::Text))
    	 m_editor->setPlainText(file.readAll());

     QDockWidget *consoleDock = new QDockWidget(tr("Console"), this);
     consoleDock->setAllowedAreas(Qt::BottomDockWidgetArea);
     m_console = new QTextEdit(consoleDock);
     consoleDock->setWidget(m_console);
     addDockWidget(Qt::BottomDockWidgetArea ,consoleDock);

     QDockWidget* outputDock = new QDockWidget(tr("Output"), this);
     outputDock->setAllowedAreas(Qt::RightDockWidgetArea | Qt::LeftDockWidgetArea);
     m_graphicsScene = new QGraphicsScene;
     m_image = new QImage(256, 256, QImage::Format_ARGB32);
     m_image->setPixel(QPoint(100,100), 0xFF000000);
     m_pixmap = new QPixmap;
     m_pixmap->convertFromImage(*m_image);

     m_pixmapItem = m_graphicsScene->addPixmap(*m_pixmap);

     m_graphicsView = new QGraphicsView(m_graphicsScene, outputDock);
     outputDock->setWidget(m_graphicsView);
     addDockWidget(Qt::RightDockWidgetArea, outputDock);

     QObject::connect(m_editor, SIGNAL(textChanged(void)),
                           this, SLOT(TextUpdated(void)));
 }

 void MainWindow::TextUpdated()
 {

	m_image = new QImage(256, 256, QImage::Format_ARGB32);
	QImage* image = MakeImage(m_editor->document()->toPlainText().toStdString(), 256, 256);

	if (image)
	{
		m_image = image;

		m_pixmap->convertFromImage(*m_image);
	}

 }

 void MainWindow::setupFileMenu()
 {
     QMenu *fileMenu = new QMenu(tr("&File"), this);
     menuBar()->addMenu(fileMenu);

     fileMenu->addAction(tr("&New"), this, SLOT(newFile()),
                         QKeySequence::New);

     fileMenu->addAction(tr("&Open..."), this, SLOT(openFile()),
                         QKeySequence::Open);

     fileMenu->addAction(tr("E&xit"), qApp, SLOT(quit()),
                         QKeySequence::Quit);
 }

 void MainWindow::setupHelpMenu()
 {
     QMenu *helpMenu = new QMenu(tr("&Help"), this);
     menuBar()->addMenu(helpMenu);

     helpMenu->addAction(tr("&About"), this, SLOT(about()));
     helpMenu->addAction(tr("About &Qt"), qApp, SLOT(aboutQt()));
 }
