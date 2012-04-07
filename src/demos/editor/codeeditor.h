//
// codeeditor.h
//

#ifndef CODEEDITOR_H_
#define CODEEDITOR_H_

#include <QTextEdit>

class Highlighter;
class QFont;

class CodeEditor : public QTextEdit
{
public:
	CodeEditor(QWidget* parent, QFont& font);
	virtual ~CodeEditor();

	void keyPressEvent (QKeyEvent* event);

private:
	Highlighter* m_highlighter;
};

#endif /* CODEEDITOR_H_ */
