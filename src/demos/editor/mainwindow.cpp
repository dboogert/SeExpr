#include "mainwindow.h"
#include <QtGui>

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
	m_image = new QImage(512, 512, QImage::Format_ARGB32);
	m_image->setPixel(QPoint(100,100), 0xFF000000);
	m_pixmap = new QPixmap;
	m_pixmap->convertFromImage(*m_image);

	m_pixmapItem = m_graphicsScene->addPixmap(*m_pixmap);

	m_graphicsView = new QGraphicsView(m_graphicsScene, outputDock);
	m_graphicsView->setBackgroundBrush(QBrush(QColor(30, 30, 30),Qt::SolidPattern));
	outputDock->setWidget(m_graphicsView);
	addDockWidget(Qt::RightDockWidgetArea, outputDock);

	QObject::connect(m_editor, SIGNAL(textChanged(void)), this, SLOT(TextUpdated(void)));
}

void MainWindow::TextUpdatedImp()
{
	std::vector<SeExpression::Error> errors;
	QImage* image = MakeImage(m_editor->document()->toPlainText().toStdString(), 512, 512, errors);

	m_console->clear();
	if (errors.size() == 0)
	{
		m_image = image;

		m_pixmap->convertFromImage(*m_image);
		m_pixmapItem->update(QRectF());
		m_console->append(QString("valid expression"));
	}

	validate(errors);
}

void MainWindow::validate(const std::vector<SeExpression::Error>& errors)
{
	ScopedBool b(m_editorValidate);

	QTextCursor cursor(m_editor->document());
	cursor.select(QTextCursor::Document);

	QTextCharFormat format;
	format.setUnderlineStyle(QTextCharFormat::NoUnderline);
	cursor.mergeCharFormat(format);

	for(size_t i = 0; i < errors.size(); ++i)
	{
		const SeExpression::Error& error = errors[i];
		std::cout << error.startPos << " " << error.endPos << " " << error.error << std::endl;
		QString posstr;

		m_console->append(QString::number(error.startPos) + ":" + QString::number(error.endPos) + " >> " + QString(error.error.c_str()));


		QTextCursor cursor(m_editor->document());
		cursor.setPosition(error.startPos);
		cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, error.endPos - error.startPos);
		QTextCharFormat format;
		format.setUnderlineStyle(QTextCharFormat::WaveUnderline);
		format.setUnderlineColor(QColor::fromRgb(255,0,0,255));
		cursor.mergeCharFormat(format);

	}
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

	fileMenu->addAction(tr("&New"), this, SLOT(newFile()),
			QKeySequence::New);

	fileMenu->addAction(tr("&Open..."), this, SLOT(openFile()),
			QKeySequence::Open);

	fileMenu->addAction(tr("&Save..."), this, SLOT(saveFile()),
			QKeySequence::Save);

	fileMenu->addAction(tr("Save As..."), this, SLOT(saveFileAs()),
			QKeySequence::SaveAs);


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

void MainWindow::updateWindowTitle()
{
	QString filename_info = m_filename;
	if (filename_info.isEmpty())
		filename_info = tr("New");

	setWindowTitle(tr("Se.Expr Editor [") + filename_info + tr("]"));
}
