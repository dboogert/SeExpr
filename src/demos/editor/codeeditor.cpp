//
// codeeditor.cpp
//

#include "codeeditor.h"
#include "highlighter.h"

#include <QKeyEvent>
#include <QTextDocumentFragment>
#include <iostream>

CodeEditor::CodeEditor(QWidget* parent, QFont& font)
: QTextEdit(parent),
  m_autoCompleteStartPosition(-1)
{
	setFont(font);
	setTextColor(QColor(200,200,200,255));
	m_highlighter = new Highlighter(document());

	QTextOption opt =  document()->defaultTextOption();
	opt.setWrapMode(QTextOption::NoWrap);
	document()->setDefaultTextOption(opt);

	m_autoCompleteWidget = new AutoCompleteWidget(this);
	m_autoCompleteWidget->hide();

	QObject::connect(m_autoCompleteWidget, SIGNAL(ItemSelected(const QString& text)), this, SLOT(AutocompleteItemSelected(const QString& text)));
	QObject::connect(this, SIGNAL(textChanged(void)), this, SLOT(TextUpdated(void)));

	setupAutoComplete();
}

CodeEditor::~CodeEditor()
{
}

void CodeEditor::TextUpdated()
{
	UpdateAutocomplete(false);
}

void CodeEditor::keyPressEvent (QKeyEvent* e)
{
	std::cout << "CodeEditor::keyPressEvent" << std::endl;
	std::cout << "key code:" << e->key() << std::endl;
	std::cout << "Return code:" << Qt::Key_Return << std::endl;

	if (e->key() == Qt::Key_Space && (e->modifiers() & Qt::MetaModifier))
	{
		AutocompleteEnabled();
		e->accept();
	}

	QTextEdit::keyPressEvent(e);
}


void CodeEditor::AutocompleteEnabled()
{
	std::cout << "Autocomplete: ";

	if (m_autoCompleteWidget->isVisible())
	{
		std::cout << " already visible" << std::endl;
		return;
	}

	std::cout << "enabling " << std::endl;


	QTextCursor c = textCursor();

	c.movePosition(QTextCursor::StartOfWord, QTextCursor::MoveAnchor, 1);
	QRect r = cursorRect(c);

	m_autoCompleteWidget->move(r.bottomLeft() + QPoint(0,5));

	m_autoCompleteWidget->show();
	m_autoCompleteWidget->setFocus();

	UpdateAutocomplete(true);

	m_autoCompleteStartPosition = textCursor().position();
}

void CodeEditor::AutocompleteDisabled()
{
	m_autoCompleteWidget->hide();
	setFocus();
}

void CodeEditor::UpdateAutocomplete(bool onEnabled)
{
	if (!m_autoCompleteWidget->isVisible())
		return;

	QTextCursor c = textCursor();
	c.movePosition(QTextCursor::StartOfWord, QTextCursor::MoveAnchor, 1);
	c.movePosition(QTextCursor::EndOfWord, QTextCursor::KeepAnchor,1);

	QString selectedText = c.selection().toPlainText();

	std::cout << "selection is empty:" << c.selection().isEmpty() << std::endl;
	std::cout << "selected text length" << selectedText.length() << std::endl;
	std::cout << "selected text: '" << selectedText.toStdString() << "'" << std::endl;

	if (!m_autoComplete.UpdateFilter(c.selectedText().toStdString()))
		return;

	const std::vector<Autocomplete::Option>& filteredOptions =  m_autoComplete.FilteredOptions();

	m_autoCompleteWidget->UpdateOptions(filteredOptions);

	std::cout << "Update Auto Complete" << std::endl;
	std::cout << "auto complete size" << filteredOptions.size() << std::endl;
	if (filteredOptions.size() == 0)
	{
		AutocompleteDisabled();
		return;
	}

	if (filteredOptions.size() == 1 && onEnabled)
	{
		const std::string& insertionString = filteredOptions[0].name;

		QTextCursor c2 = textCursor();
		c2.movePosition(QTextCursor::StartOfWord, QTextCursor::MoveAnchor, 1);
		c2.movePosition(QTextCursor::EndOfWord, QTextCursor::KeepAnchor,1);

		AutocompleteDisabled();
		c2.insertText(QString(tr(insertionString.c_str())));
		return;
	}

	if (textCursor().position() < m_autoCompleteStartPosition)
	{
		AutocompleteDisabled();
		return;
	}
}

void CodeEditor::AutocompleteItemSelected(const QString& text)
{
	if (text.size() > 0 )
	{
		QTextCursor c2 = textCursor();
		c2.movePosition(QTextCursor::StartOfWord, QTextCursor::MoveAnchor, 1);
		c2.movePosition(QTextCursor::EndOfWord, QTextCursor::KeepAnchor,1);

		m_autoCompleteWidget->hide();

		c2.insertText(text);

		c2.movePosition(QTextCursor::StartOfWord, QTextCursor::MoveAnchor, 1);
		setFocus();
	}
	m_autoCompleteWidget->hide();
	setFocus();
}


void CodeEditor::setupAutoComplete()
{
	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "clamp"));
	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "compress"));
	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "expand"));
	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "contrast"));
	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "invert"));

	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "remap"));
	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "hsi"));
	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "midhsi"));

	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "rgbtohsl"));
	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "hsltorgb"));

	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "bias"));
	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "gamma"));

	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "fit"));
	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "mix"));

	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "boxstep"));
	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "gaussstep"));
	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "linearstep"));
	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "smoothstep"));

	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "rand"));
	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "hash"));

	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "cellnoise"));
	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "cellnoise2"));
	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "cellnoise3"));

	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "noise"));
	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "cnoise"));
	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "snoise"));

	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "cnoise4"));
	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "snoise4"));
	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "vnoise4"));

	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "pnoise"));

	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "cperlin"));
	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "sperlin"));
	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "vperlin"));

	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "fbm"));
	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "cfbm"));
	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "vfbm"));
	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "fbm4"));
	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "cfbm4"));
	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "vfbm4"));

	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "turbulence"));
	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "cturbulence"));
	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "vturbulence"));

	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "cycle"));
	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "pick"));
	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "choose"));
	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "wchoose"));

	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Constant, "PI"));
	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Constant, "E"));

	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "abs"));
	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "max"));
	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "min"));
	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "fmod"));

	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "cbrt"));
	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "sqrt"));

	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "ceil"));
	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "floor"));
	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "round"));
	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "trunc"));

	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "exp"));
	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "log"));
	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "log10"));
	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "pow"));

	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "acos"));
	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "asin"));
	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "atan"));
	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "atan2"));

	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "cos"));
	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "sin"));
	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "tan"));

	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "acosd"));
	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "asind"));
	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "atand"));
	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "atan2d"));

	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "cosd"));
	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "sind"));
	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "tand"));

	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "acosh"));
	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "asinh"));
	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "atanh"));

	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "cosh"));
	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "sinh"));
	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "tanh"));

	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "deg"));
	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "rad"));

	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "hypot"));

	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "angle"));
	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "cross"));
	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "dist"));
	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "dot"));
	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "length"));
	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "norm"));
	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "ortho"));
	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "up"));
	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "rotate"));

	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "curve"));
	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "spline"));

	m_autoComplete.AddItem(Autocomplete::Option(Autocomplete::Function, "printf"));
}
