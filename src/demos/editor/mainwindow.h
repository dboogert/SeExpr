#pragma once

#include "highlighter.h"
#include "imager.h"
#include "autocomplete.h"

#include <SeExpression.h>

#include <QMainWindow>

class CodeEditor;
class QTextEdit;
class QGraphicsView;
class QGraphicsScene;
class QPixmap;
class QGraphicsPixmapItem;
class QListWidget;
class QListWidgetItem;
class AutoCompleteListWidget;
class QDockWidget;

class MainWindow : public QMainWindow
{
	Q_OBJECT;
public:
	MainWindow(QWidget* parent = 0);
	virtual ~MainWindow() {}
public slots:
	void about();
	void newFile();
	void openFile(const QString& path = QString());
	void saveFileAs();
	void saveFile();
	void saveImage();
	void TextUpdated();
	void ImageUpdated();
	void ShowOutputWindow();
	void ShowConsoleWindow();
private:
	void setupEditor();
	void setupFileMenu();
	void setupHelpMenu();

	void updateWindowTitle();
	void validate(const std::vector<SeExpression::Error>& errors);

	CodeEditor* m_editor;
	bool m_editorValidate;

	QTextEdit* m_console;

	QGraphicsView* m_graphicsView;
	QGraphicsScene* m_graphicsScene;

	QPixmap* m_pixmap;
	QImage* m_image;
	QGraphicsPixmapItem* m_pixmapItem;

	Imager* m_imager;
	QString m_filename;

	QDockWidget* m_outputDock;
	QDockWidget* m_consoleDock;

	QTimer* m_timer;

	std::vector<SeExpression::Error> m_lastDisplayedErrors;
};
