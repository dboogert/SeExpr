#pragma once

#include <QMainWindow>

#include <SeExpression.h>
#include "highlighter.h"

class QTextEdit;
class QGraphicsView;
class QGraphicsScene;
class QPixmap;
class QGraphicsPixmapItem;

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
	void TextUpdated();

private:
	void setupEditor();
	void setupFileMenu();
	void setupHelpMenu();

	void TextUpdatedImp();

	void updateWindowTitle();
	void validate(const std::vector<SeExpression::Error>& errors);

	QTextEdit* m_editor;
	bool m_editorValidate;

	QTextEdit* m_console;

	QGraphicsView* m_graphicsView;
	QGraphicsScene* m_graphicsScene;

	QPixmap* m_pixmap;
	QImage* m_image;
	QGraphicsPixmapItem* m_pixmapItem;

	Highlighter* m_highlighter;

	QString m_filename;


};
