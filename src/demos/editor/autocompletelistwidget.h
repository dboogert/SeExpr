//
// autocompletelistwidget.h
//

#ifndef AUTOCOMPLETELISTWIDGET_H_
#define AUTOCOMPLETELISTWIDGET_H_

#include <QListWidget>

#include "autocomplete.h"

class QTextEdit;
class QFocusEvent;

class AutoCompleteListWidget : public QListWidget
{
	Q_OBJECT;
public:
	explicit AutoCompleteListWidget(QTextEdit *parent = 0);
	virtual ~AutoCompleteListWidget();

	void keyPressEvent (QKeyEvent* event);

private:
	QTextEdit* m_editor;

	Autocomplete m_autoComplete;
	bool m_autoCompleteActive;
	int m_autoCompleteStartPosition;
};

#endif /* AUTOCOMPLETELISTWIDGET_H_ */
