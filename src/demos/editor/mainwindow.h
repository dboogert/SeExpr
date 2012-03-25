/*
 * MainWindow.h
 *
 *  Created on: Mar 12, 2012
 *      Author: dboogert
 */

#pragma once

#include <QMainWindow>
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
	void TextUpdated();
private:
	void setupEditor();
	void setupFileMenu();
	void setupHelpMenu();

	QTextEdit* m_editor;
	QTextEdit* m_console;

	QGraphicsView* m_graphicsView;
	QGraphicsScene* m_graphicsScene;

	QPixmap* m_pixmap;
	QImage* m_image;
	QGraphicsPixmapItem* m_pixmapItem;

	Highlighter* m_highlighter;
};
