#include "mainwindow.h"
#include "imager.h"


#include <QtGui>
#include <QWebView>
#include <QWebFrame>
#include <QWebElement>

#include <iostream>

bool AutoCompleteKeyboardIntercept::eventFilter(QObject *obj, QEvent *event)
{
	if (event->type() == QEvent::KeyPress)
	{
		QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
		qDebug("Ate key press %d", keyEvent->key());
		return true;
	}
	else
	{
		// standard event processing
		return QObject::eventFilter(obj, event);
	}
}


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
  m_editorValidate(false),
  m_autoCompleteActive(false),
  m_autoCompleteStartPosition(-1)
{
	setupFileMenu();
	setupHelpMenu();
	setupEditor();
	setupAutoComplete();

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

	if (!fileName.isEmpty())
	{
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
	m_editor->setTextColor(QColor(200,200,200,255));
	m_highlighter = new Highlighter(m_editor->document());

	QTextOption opt =  m_editor->document()->defaultTextOption();
	opt.setWrapMode(QTextOption::NoWrap);
	m_editor->document()->setDefaultTextOption(opt);

	QDockWidget *consoleDock = new QDockWidget(tr("Console"), this);
	consoleDock->setAllowedAreas(Qt::BottomDockWidgetArea);
	m_console = new QTextEdit(consoleDock);
	m_console->setReadOnly(true);
	m_console->setFont(font);
	m_console->setTextColor(QColor(200,200,200,255));
	consoleDock->setWidget(m_console);
	addDockWidget(Qt::BottomDockWidgetArea ,consoleDock);

	QDockWidget* outputDock = new QDockWidget(tr("Output"), this);
	outputDock->setAllowedAreas(Qt::RightDockWidgetArea | Qt::LeftDockWidgetArea);
	m_graphicsScene = new QGraphicsScene;
	m_pixmap = new QPixmap;

	int width = 512;
	int height = 512;

	m_imager = new Imager(width, height);

	m_pixmapItem = m_graphicsScene->addPixmap(*m_pixmap);

	m_graphicsView = new QGraphicsView(m_graphicsScene, outputDock);
	m_graphicsView->setMinimumSize(width + 32, height +32);
	m_graphicsView->setBackgroundBrush(QBrush(QColor(48, 48, 48), Qt::SolidPattern));
	outputDock->setWidget(m_graphicsView);
	addDockWidget(Qt::RightDockWidgetArea, outputDock);

	m_timer = new QTimer(this);
	connect(m_timer, SIGNAL(timeout()), this, SLOT(ImageUpdated()));
	m_timer->start(500);

	QObject::connect(m_editor, SIGNAL(textChanged(void)), this, SLOT(TextUpdated(void)));

	m_webView = new QWebView();
	//m_webView->load(QUrl("http://www.google.com"));
	QWebFrame* frame = m_webView->page()->mainFrame();
	frame->setHtml(tr("<html><h1>Function name</h1></html>"));
	//frame->documentElement().appendInside("<h1>test</h1>");

//	QDockWidget* helpDock = new QDockWidget(tr("Help"), this);
//	helpDock->setAllowedAreas(Qt::RightDockWidgetArea);
//	helpDock->setWidget(m_webView);
//	addDockWidget(Qt::RightDockWidgetArea, helpDock);

	m_autoCompleteList = new QListWidget(m_editor);
	m_autoCompleteList->setFixedSize(QSize(200,500));
	m_autoCompleteList->hide();

	QObject::connect(m_editor, SIGNAL(cursorPositionChanged()), SLOT(CursorPositionChanged(void)));

//	AutoCompleteKeyboardIntercept *autocompleteIntercept = new AutoCompleteKeyboardIntercept();
//	m_editor->installEventFilter(autocompleteIntercept);
}

void MainWindow::TextUpdatedImp()
{
	std::string expression = m_editor->document()->toPlainText().toStdString();

	if (m_autoCompleteActive)
	{
		UpdateAutocomplete(false);
	}

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

	m_console->clear();

	if (errors.size() == 0)
	{
		m_console->append(QString("valid expression"));
	}

	QTextCursor cursor(m_editor->document());
	cursor.select(QTextCursor::Document);

	QTextCharFormat format;
	format.setForeground(Qt::white);
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

void MainWindow::UpdateAutocomplete(bool onEnabled)
{
	QTextCursor c = m_editor->textCursor();
	c.movePosition(QTextCursor::StartOfWord, QTextCursor::MoveAnchor, 1);
	c.movePosition(QTextCursor::EndOfWord, QTextCursor::KeepAnchor,1);

	m_autoComplete.UpdateFilter(c.selectedText().toStdString());
	const std::vector<Autocomplete::Option>& filteredOptions =  m_autoComplete.FilteredOptions();
	m_autoCompleteList->clear();
	for (size_t i = 0; i < filteredOptions.size(); ++i)
	{
		QListWidgetItem* item = new QListWidgetItem(tr(filteredOptions[i].name.c_str()), m_autoCompleteList);
		m_autoCompleteList->addItem(item);
	}

	if (filteredOptions.size() == 0)
	{
		m_autoCompleteActive = false;
		m_autoCompleteList->hide();
	}

	if (filteredOptions.size() == 1 && onEnabled)
	{
		const std::string& insertionString = filteredOptions[0].name;

		QTextCursor c2 = m_editor->textCursor();
		c2.movePosition(QTextCursor::StartOfWord, QTextCursor::MoveAnchor, 1);
		c2.movePosition(QTextCursor::EndOfWord, QTextCursor::KeepAnchor,1);

		m_autoCompleteActive = false;
		m_autoCompleteList->hide();

		c2.insertText(QString(tr(insertionString.c_str())));
	}

	if (m_editor->textCursor().position() < m_autoCompleteStartPosition)
	{
		AutocompleteDisabled();
	}
}

void MainWindow::AutocompleteEnabled()
{
	std::cout << "Autocomplete " << std::endl;

	QTextCursor c = m_editor->textCursor();

	c.movePosition(QTextCursor::StartOfWord, QTextCursor::MoveAnchor, 1);
	QRect r = m_editor->cursorRect(c);
	m_autoCompleteList->move(r.bottomLeft());

	m_autoCompleteList->show();
	m_autoCompleteActive = true;

	UpdateAutocomplete(true);

	m_autoCompleteStartPosition = m_editor->textCursor().position();
}

void MainWindow::AutocompleteDisabled()
{
	m_autoCompleteList->hide();
	m_autoCompleteActive = false;
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
	fileMenu->addAction(tr("Auto Complete"), this, SLOT(AutocompleteEnabled()), QKeySequence(Qt::META + Qt::Key_Space));
	fileMenu->addAction(tr("Leave Autocomplete"), this, SLOT(AutocompleteDisabled()), QKeySequence(Qt::Key_Escape));
	fileMenu->addAction(tr("E&xit"), qApp, SLOT(quit()), QKeySequence::Quit);
}

void MainWindow::setupHelpMenu()
{
	QMenu *helpMenu = new QMenu(tr("&Help"), this);
	menuBar()->addMenu(helpMenu);

	helpMenu->addAction(tr("&About"), this, SLOT(about()));
	helpMenu->addAction(tr("About &Qt"), qApp, SLOT(aboutQt()));
}

void MainWindow::setupAutoComplete()
{
	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "clamp"));
	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "compress"));
	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "expand"));
	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "contrast"));
	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "invert"));

	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "remap"));
	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "hsi"));
	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "midhsi"));

	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "rgbtohsl"));
	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "hsltorgb"));

	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "bias"));
	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "gamma"));

	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "fit"));
	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "mix"));

	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "boxstep"));
	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "gaussstep"));
	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "linearstep"));
	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "smoothstep"));

	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "rand"));
	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "hash"));

	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "cellnoise"));
	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "cellnoise2"));
	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "cellnoise3"));

	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "noise"));
	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "cnoise"));
	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "snoise"));

	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "cnoise4"));
	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "snoise4"));
	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "vnoise4"));

	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "pnoise"));

	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "cperlin"));
	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "sperlin"));
	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "vperlin"));

	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "fbm"));
	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "cfbm"));
	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "vfbm"));
	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "fbm4"));
	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "cfbm4"));
	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "vfbm4"));

	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "turbulence"));
	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "cturbulence"));
	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "vturbulence"));

	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "cycle"));

	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "pick"));
	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "choose"));
	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "wchoose"));

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

	validate(m_imager->GetErrors());
}

void MainWindow::CursorPositionChanged()
{
}

