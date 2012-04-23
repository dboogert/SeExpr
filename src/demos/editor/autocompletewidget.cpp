//
// autocompletewidget.cpp
//

#include "autocompletewidget.h"
#include "autocompletelistwidget.h"

#include <QWebView>
#include <QBoxLayout>
#include <QFocusEvent>

AutoCompleteWidget::AutoCompleteWidget(QWidget* parent)
: QWidget(parent)
{
	this->setMinimumWidth(350);
	this->setMaximumWidth(450);

	this->setMinimumHeight(250);
	this->setMaximumHeight(300);

	m_layout = new QBoxLayout(QBoxLayout::LeftToRight, this);
	m_layout->setContentsMargins(0,0,0,0);
	m_autoCompleteList = new AutoCompleteListWidget();
	m_autoCompleteList->setMinimumWidth(100);

	m_webView = new QWebView();
	m_webView->load(QUrl("file:////Users/dboogert/work/SeExpr/src/demos/editor/test.html"));

	m_autoCompleteList->setFocus();

	m_layout->addWidget(m_autoCompleteList);
	m_layout->addWidget(m_webView);

	QObject::connect(m_autoCompleteList, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(AutocompleteItemSelected(QListWidgetItem *)));
}

AutoCompleteWidget::~AutoCompleteWidget()
{
}

void AutoCompleteWidget::AutocompleteItemSelected(QListWidgetItem * item)
{
	QList<QListWidgetItem*> items = m_autoCompleteList->selectedItems();

	if (items.size() > 0)
	{
		emit ItemSelected(items[0]->text());
	}
}

void AutoCompleteWidget::UpdateOptions(const std::vector<Autocomplete::Option>& options)
{
	m_autoCompleteList->clear();

	for (size_t i = 0; i < options.size(); ++i)
	{
		QListWidgetItem* item = new QListWidgetItem(tr(options[i].name.c_str()), m_autoCompleteList);
		m_autoCompleteList->addItem(item);
	}

	m_autoCompleteList->setSelectionMode(QAbstractItemView::SingleSelection);
	m_autoCompleteList->setCurrentItem(m_autoCompleteList->item(0));
}

void AutoCompleteWidget::focusOutEvent (QFocusEvent * event)
{
//	if (event->lostFocus())
//	{
//		this->close();
//	}
}

