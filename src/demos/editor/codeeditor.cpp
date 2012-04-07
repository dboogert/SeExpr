//
// codeeditor.cpp
//

#include "codeeditor.h"
#include "highlighter.h"

CodeEditor::CodeEditor(QWidget* parent, QFont& font)
: QTextEdit(parent)
{
	setFont(font);
	setTextColor(QColor(200,200,200,255));
	m_highlighter = new Highlighter(document());

	QTextOption opt =  document()->defaultTextOption();
	opt.setWrapMode(QTextOption::NoWrap);
	document()->setDefaultTextOption(opt);
}

CodeEditor::~CodeEditor()
{
}

void CodeEditor::keyPressEvent (QKeyEvent* e)
{
	QTextEdit::keyPressEvent(e);
}

