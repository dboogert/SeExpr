/*
 * highlighter.h
 *
 *  Created on: Mar 12, 2012
 *      Author: dboogert
 */

#pragma once

#include <QSyntaxHighlighter>
#include <QHash>
#include <QTextCharFormat>

class QTextDocument;

class Highlighter : public QSyntaxHighlighter
{
	Q_OBJECT;
public:
	Highlighter(QTextDocument* parent = 0);
protected:
	void highlightBlock(const QString& text);
private:
	struct HighlightingRule
	{
		QRegExp pattern;
		QTextCharFormat format;
	};

	QVector<HighlightingRule> highlightingRules;

	QTextCharFormat keywordFormat;
	QTextCharFormat singleLineCommentFormat;
	QTextCharFormat quotationFormat;
	QTextCharFormat functionFormat;
};
