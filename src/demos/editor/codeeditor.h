//
// codeeditor.h
//

#ifndef CODEEDITOR_H_
#define CODEEDITOR_H_

#include "autocomplete.h"
#include "autocompletelistwidget.h"
#include "autocompletewidget.h"

#include <QTextEdit>


class Highlighter;
class QFont;

class CodeEditor : public QTextEdit
{
	Q_OBJECT;
public:
	CodeEditor(QWidget* parent, QFont& font);
	virtual ~CodeEditor();

	void keyPressEvent (QKeyEvent* event);
	void UpdateAutocomplete(bool onEnabled);

public slots:
	void AutocompleteEnabled();
	void AutocompleteDisabled();
	void AutocompleteItemSelected(const QString& text);

	void TextUpdated();

private:
	void setupAutoComplete();

	Highlighter* m_highlighter;
	Autocomplete m_autoComplete;
	AutoCompleteWidget* m_autoCompleteWidget;
	int m_autoCompleteStartPosition;
};

#endif /* CODEEDITOR_H_ */
