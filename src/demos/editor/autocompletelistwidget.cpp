//
// autocompletelistwidget.cpp
//

#include "autocompletelistwidget.h"

#include <QKeyEvent>
#include <QTextEdit>
#include <QTextCursor>
#include <QFocusEvent>

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

	std::cout << Qt::Key_Return << std::endl;


	if (event->key() == 16777220)
	{
		std::cout << "auto complete item selected" << std::endl;
		event->accept();
//		QTextCursor c2 = m_editor->textCursor();
//		close();

		QList<QListWidgetItem*> selectedItems = this->selectedItems();

		if (!selectedItems.empty())
		{
			emit itemDoubleClicked(selectedItems.first());
		}
		return;
	}
	else if (event->key() == Qt::Key_Backspace)
	{
		event->accept();

		//QTextCursor c2 = m_editor->textCursor();
		//c2.deleteChar();
	}
	else if (event->key() == Qt::Key_Space)
	{
		//QTextCursor c2 = m_editor->textCursor();
		//c2.insertText(event->text());
		//this->parentWidget()->setFocus();
		event->accept();
	}
	else if (event->key() == Qt::Key_Escape)
	{
		//this->parentWidget()->setFocus();
		event->accept();
	}
	else if (event->text().isSimpleText())
	{
		//QTextCursor c2 = m_editor->textCursor();
		//c2.insertText(event->text());
	}

	QListWidget::keyPressEvent(event);
}

