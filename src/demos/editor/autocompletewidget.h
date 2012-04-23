//
// autocompletewidget.h
//

#ifndef AUTOCOMPLETEWIDGET_H_
#define AUTOCOMPLETEWIDGET_H_

#include "autocompletelistwidget.h"
#include "autocomplete.h"

#include <QWidget>

class QWebView;
class QBoxLayout;
class QListWidgetItem;
class QFocusEvent;

class AutoCompleteWidget : public QWidget
{
	Q_OBJECT;

public:
	AutoCompleteWidget(QWidget* parent = NULL);
	virtual ~AutoCompleteWidget();

	void UpdateOptions(const std::vector<Autocomplete::Option>& options);

	void focusOutEvent (QFocusEvent * event);
public slots:
	void AutocompleteItemSelected(QListWidgetItem * item);

signals:
     void ItemSelected(const QString& text);

private:

	QBoxLayout* m_layout;
	AutoCompleteListWidget* m_autoCompleteList;
	QWebView* m_webView;
};

#endif /* AUTOCOMPLETEWIDGET_H_ */
