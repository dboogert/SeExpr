//
// autocompletelistwidget.h
//

#ifndef AUTOCOMPLETELISTWIDGET_H_
#define AUTOCOMPLETELISTWIDGET_H_

#include <QListWidget>

class QTextEdit;

class AutoCompleteListWidget : public QListWidget
{
public:
	explicit AutoCompleteListWidget(QTextEdit *parent = 0);
	virtual ~AutoCompleteListWidget();

	void keyPressEvent (QKeyEvent* event);

private:
	QTextEdit* m_editor;
};

#endif /* AUTOCOMPLETELISTWIDGET_H_ */
