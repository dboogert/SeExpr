#include "mainwindow.h"
#include <QtGui>
#include <QWebView>

#include <iostream>
#include "imager.h"

struct ScopedBool
{
	bool& m_value;

	ScopedBool(bool& value)
	: m_value(value)
	{
		m_value = true;
	}

	~ScopedBool()
	{
		m_value = false;
	}
};

MainWindow::MainWindow(QWidget *parent)
: QMainWindow(parent),
  m_editorValidate(false)
{
	setupFileMenu();
	setupHelpMenu();
	setupEditor();

	setCentralWidget(m_editor);
	updateWindowTitle();
}

void MainWindow::about()
{
	QMessageBox::about(this, tr("About Se.Expr Editor"),
			tr("<p>Demo application for experimenting with SeExpressions</p>"));
}

void MainWindow::newFile()
{
	m_editor->clear();
	m_filename.clear();
	updateWindowTitle();
	TextUpdatedImp();
}

void MainWindow::openFile(const QString &path)
{
	QString fileName = path;

	if (fileName.isNull())
		fileName = QFileDialog::getOpenFileName(this,
				tr("Open File"), "", "Se.Expr Files (*.se)");

	if (!fileName.isEmpty()) {
		m_filename = fileName;
		updateWindowTitle();
		QFile file(fileName);
		if (file.open(QFile::ReadOnly | QFile::Text))
			m_editor->setPlainText(file.readAll());
	}
}

void MainWindow::saveFile()
{
	if (!m_filename.isEmpty())
	{
		QFile file(m_filename);
		if (file.open(QFile::WriteOnly | QFile::Text))
		{
			QByteArray barray;
			barray.append(m_editor->document()->toPlainText());
			file.write(barray);
		}
	}
	else
	{
		saveFileAs();
	}
}


void MainWindow::saveFileAs()
{
	QString fileName;

	if (fileName.isNull())
		fileName = QFileDialog::getSaveFileName(this,
				tr("Save File"), "", "Se.Expr Files (*.se)");

	if (!fileName.isEmpty()) {
		m_filename = fileName;
		updateWindowTitle();
		QFile file(fileName);
		if (file.open(QFile::WriteOnly | QFile::Text))
		{
			QByteArray barray;
			barray.append(m_editor->document()->toPlainText());
			file.write(barray);
		}
	}
}

void MainWindow::saveImage()
{
	QString fileName;

	if (fileName.isNull())
		fileName = QFileDialog::getSaveFileName(this,
				tr("Save Image File"), "", "PNG files (*.png)");

	if (!fileName.isEmpty())
	{
		m_image->save(fileName);
	}
}


void MainWindow::setupEditor()
{
	QFont font;
	font.setFamily("Courier");
	font.setFixedPitch(true);
	font.setPointSize(14);

	m_editor = new QTextEdit;
	m_editor->setFont(font);

	m_highlighter = new Highlighter(m_editor->document());

	QTextOption opt =  m_editor->document()->defaultTextOption();
	opt.setWrapMode(QTextOption::NoWrap);
	m_editor->document()->setDefaultTextOption(opt);

	QDockWidget *consoleDock = new QDockWidget(tr("Console"), this);
	consoleDock->setAllowedAreas(Qt::BottomDockWidgetArea);
	m_console = new QTextEdit(consoleDock);
	m_console->setReadOnly(true);
	m_console->setFont(font);
	consoleDock->setWidget(m_console);
	addDockWidget(Qt::BottomDockWidgetArea ,consoleDock);

	QDockWidget* outputDock = new QDockWidget(tr("Output"), this);
	outputDock->setAllowedAreas(Qt::RightDockWidgetArea | Qt::LeftDockWidgetArea);
	m_graphicsScene = new QGraphicsScene;
	m_pixmap = new QPixmap;

	m_imager = new Imager(1024, 1024);

	m_pixmapItem = m_graphicsScene->addPixmap(*m_pixmap);

	m_graphicsView = new QGraphicsView(m_graphicsScene, outputDock);
	m_graphicsView->setBackgroundBrush(QBrush(QColor(30, 30, 30),Qt::SolidPattern));
	outputDock->setWidget(m_graphicsView);
	addDockWidget(Qt::RightDockWidgetArea, outputDock);

	m_timer = new QTimer(this);
	connect(m_timer, SIGNAL(timeout()), this, SLOT(ImageUpdated()));
	m_timer->start(500);

	QObject::connect(m_editor, SIGNAL(textChanged(void)), this, SLOT(TextUpdated(void)));

	m_webView = new QWebView();
	m_webView->load(QUrl("http://www.google.com"));

	QDockWidget* helpDock = new QDockWidget(tr("Help"), this);
	helpDock->setAllowedAreas(Qt::RightDockWidgetArea | Qt::LeftDockWidgetArea);
	helpDock->setWidget(m_webView);
	addDockWidget(Qt::RightDockWidgetArea, helpDock);
}

void MainWindow::TextUpdatedImp()
{
	std::string expression = m_editor->document()->toPlainText().toStdString();

	m_imager->UpdateExpression(expression);
}

bool operator==(const SeExpression::Error& lhs, const SeExpression::Error& rhs)
{
	if (lhs.endPos != rhs.endPos)
		return false;

	if (lhs.startPos != rhs.startPos)
		return false;

	if (lhs.error != rhs.error)
		return false;

	return true;
}

void MainWindow::validate(const std::vector<SeExpression::Error>& errors)
{
	ScopedBool b(m_editorValidate);

//	if (m_lastDisplayedErrors == errors)
//		return;

	m_console->clear();

	if (errors.size() == 0)
	{

		m_console->append(QString("valid expression"));
	}

	QTextCursor cursor(m_editor->document());
	cursor.select(QTextCursor::Document);

	QTextCharFormat format;
	format.setUnderlineStyle(QTextCharFormat::NoUnderline);
	cursor.mergeCharFormat(format);

	for(size_t i = 0; i < errors.size(); ++i)
	{
		const SeExpression::Error& error = errors[i];
		QString posstr;

		m_console->append(QString::number(error.startPos) + ":" + QString::number(error.endPos) + " >> " + QString(error.error.c_str()));

		cursor.setPosition(error.startPos);
		cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, error.endPos - error.startPos);
		QTextCharFormat format;
		format.setUnderlineStyle(QTextCharFormat::WaveUnderline);
		format.setUnderlineColor(QColor::fromRgb(255,0,0,255));
		cursor.mergeCharFormat(format);
	}

	m_editor->update(QRect());

	m_lastDisplayedErrors = errors;
}
void MainWindow::TextUpdated()
{
	if(m_editorValidate)
		return;

	TextUpdatedImp();
}

void MainWindow::setupFileMenu()
{
	QMenu *fileMenu = new QMenu(tr("&File"), this);
	menuBar()->addMenu(fileMenu);

	fileMenu->addAction(tr("&New"), this, SLOT(newFile()), QKeySequence::New);
	fileMenu->addAction(tr("&Open..."), this, SLOT(openFile()), QKeySequence::Open);
	fileMenu->addAction(tr("&Save..."), this, SLOT(saveFile()), QKeySequence::Save);
	fileMenu->addAction(tr("Save As..."), this, SLOT(saveFileAs()), QKeySequence::SaveAs);
	fileMenu->addAction(tr("Save Image..."), this, SLOT(saveImage()));
	fileMenu->addAction(tr("E&xit"), qApp, SLOT(quit()), QKeySequence::Quit);
}

void MainWindow::setupHelpMenu()
{
	QMenu *helpMenu = new QMenu(tr("&Help"), this);
	menuBar()->addMenu(helpMenu);

	helpMenu->addAction(tr("&About"), this, SLOT(about()));
	helpMenu->addAction(tr("About &Qt"), qApp, SLOT(aboutQt()));
}

void MainWindow::updateWindowTitle()
{
	QString filename_info = m_filename;
	if (filename_info.isEmpty())
		filename_info = tr("New");

	setWindowTitle(tr("Se.Expr Editor [") + filename_info + tr("]"));
}

void MainWindow::ImageUpdated()
{
	QPixmap* pixmap = m_imager->GetPixmap();

	if (pixmap)
	{
		//delete m_pixmap;
		m_pixmapItem->setPixmap(*pixmap);
		m_pixmapItem->update(QRectF());
	}
//	m_image = m_imager->GetImage();
//
//	if(m_image)
//	{
//		m_pixmap->convertFromImage(*m_image);
//		m_pixmapItem->update(QRectF());
//	}

	validate(m_imager->GetErrors());

}
