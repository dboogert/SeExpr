//
// autocompletelistwidget.cpp
//

#include "autocompletelistwidget.h"

#include <QKeyEvent>
#include <QTextEdit>
#include <qtextCursor>

#include <iostream>

AutoCompleteListWidget::AutoCompleteListWidget(QTextEdit *parent)
: QListWidget(parent),
  m_editor(parent)
{
	setFocusPolicy(Qt::StrongFocus);
}

AutoCompleteListWidget::~AutoCompleteListWidget()
{
}

void AutoCompleteListWidget::keyPressEvent (QKeyEvent* event)
{
	std::cout << event->text().toStdString() << std::endl;
	std::cout << event->key() << std::endl;

	if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Select)
	{
		std::cout << "auto complete item selected" << std::endl;
		event->accept();
	}

	else if (event->key() == Qt::Key_Backspace)
	{
		event->accept();
		QTextCursor c2 = m_editor->textCursor();
		c2.deleteChar();
	}
	else
	{
		QTextCursor c2 = m_editor->textCursor();
		c2.insertText(event->text());
	}

	QListWidget::keyPressEvent(event);
}
