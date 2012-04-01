#pragma once

#include "highlighter.h"
#include "imager.h"
#include "autocomplete.h"

#include <SeExpression.h>

#include <QMainWindow>

class QTextEdit;
class QGraphicsView;
class QGraphicsScene;
class QPixmap;
class QGraphicsPixmapItem;
class QWebView;
class QListWidget;


class AutoCompleteKeyboardIntercept : public QObject
{
	Q_OBJECT;

signals:
	void NextItem();
	void PreviousItem();

protected:
	bool eventFilter(QObject *obj, QEvent *event);
};


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
	void CursorPositionChanged();
	void AutocompleteEnabled();
	void AutocompleteDisabled();
private:
	void setupEditor();
	void setupFileMenu();
	void setupHelpMenu();
	void setupAutoComplete();

	void TextUpdatedImp();
	void UpdateAutocomplete(bool onEnabled);

	void updateWindowTitle();
	void validate(const std::vector<SeExpression::Error>& errors);

	QTextEdit* m_editor;
	bool m_editorValidate;

	QTextEdit* m_console;

	QGraphicsView* m_graphicsView;
	QGraphicsScene* m_graphicsScene;

	QWebView* m_webView;

	QPixmap* m_pixmap;
	QImage* m_image;
	QGraphicsPixmapItem* m_pixmapItem;

	Highlighter* m_highlighter;
	Imager* m_imager;
	QString m_filename;

	QTimer* m_timer;

	Autocomplete m_autoComplete;
	QListWidget* m_autoCompleteList;
	std::vector<SeExpression::Error> m_lastDisplayedErrors;

	bool m_autoCompleteActive;
	int m_autoCompleteStartPosition;
};
