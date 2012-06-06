#include <QtGui>

#include "highlighter.h"
#include <iostream>
Highlighter::Highlighter(QTextDocument *parent)
: QSyntaxHighlighter(parent)
{
	HighlightingRule rule;


	keywordFormat.setForeground(QColor(100,100,200));
	keywordFormat.setFontWeight(QFont::Bold);
	QStringList keywordPatterns;
	keywordPatterns
	// Color, Masking,and Remapping Functions
	<< "\\bclamp\\b" << "\\bcompress\\b" << "\\bexpand\\b"
	<< "\\bcontrast\\b" << "\\binvert\\b" << "\\bremap\\b"
	<< "\\bhsi\\b" << "\\bmidhsi\\b" << "\\brgbtohsl\\b"
	<< "\\bhsltorgb\\b" << "\\bbias\\b" << "\\bgamma\\b"
	<< "\\bfit\\b" << "\\bmix\\b" << "\\bboxstep\\b"
	<< "\\bgaussstep\\b" << "\\blinearstep\\b" << "\\bsmoothstep\\b"
	// Noise functions
	<< "\\brand\\b" << "\\bhash\\b" << "\\bcellnoise\\b"
	<< "\\bcellnoise2\\b" << "\\bcellnoise3\\b" << "\\bccellnoise\\b"
	<< "\\bnoise\\b" << "\\bcnoise\\b" << "\\bsnoise\\b"
	<< "\\bvnoise\\b" << "\\bcnoise4\\b" << "\\bsnoise4\\b"
	<< "\\bvnoise4\\b" << "\\bpnoise\\b" << "\\bperlin\\b"
	<< "\\bcperlin\\b" << "\\bsperlin\\b" << "\\bvperlin\\b"
	<< "\\bfbm\\b" << "\\bcfbm\\b" << "\\bvfmb\\b"
	<< "\\bfbm4\\b" << "\\bcfmb4\\b" << "\\bvfmb4\\b"
	<< "\\bturbulence\\b" << "\\bcturbulence\\b" << "\\bvturbulence\\b"
	<< "\\bvoronoi\\b" << "\\bcvoronoi\\b" << "\\bvvoronoi\\b"
	// Selection functions
	<< "\\bcycle\\b" << "\\bpick\\b" << "\\bchoose\\b"
	<< "\\bwchoose\\b"
	// Math
	<< "\\bPI\\b" << "\\bE\\b" << "\\babs\\b"
	<< "\\bmax\\b" << "\\bmin\\b" << "\\bfmod\\b"
	<< "\\bcbrt\\b" << "\\bsqrt\\b" << "\\bceil\\b"
	<< "\\bfloor\\b" << "\\bround\\b" << "\\btrunc\\b"
	<< "\\bexp\\b" << "\\blog\\b" << "\\blog10\\b"
	<< "\\bpow\\b"
	// Trig
	<< "\\bacos\\b" << "\\basin\\b" << "\\batan\\b"
	<< "\\batan2\\b" << "\\bcos\\b" << "\\bsin\\b"
	<< "\\btan\\b" << "\\bacosd\\b" << "\\basind\\b"
	<< "\\batand\\b" << "\\batan2d\\b" << "\\bcosd\\b"
	<< "\\bsind\\b" << "\\btand\\b" << "\\bacosh\\b"
	<< "\\basinh\\b" << "\\batanh\\b" << "\\bcosh\\b"
	<< "\\bsinh\\b" << "\\btanh\\b" << "\\bdeg\\b"
	<< "\\brad\\b" << "\\bhypot\\b"
	// Vector
	<< "\\bangle\\b" << "\\bcross\\b" << "\\bdist\\b"
	<< "\\bdot\\b" << "\\blength\\b" << "\\bnorm\\b"
	<< "\\bortho\\b" << "\\bup\\b" << "\\brotate\\b"
	// Curve
	<< "\\bcurve\\b" << "\\bspline\\b" << "\\bprintf\\b"
	;
	foreach (const QString &pattern, keywordPatterns) {
		rule.pattern = QRegExp(pattern);
		rule.format = keywordFormat;
		highlightingRules.append(rule);
	}

	quotationFormat.setForeground(QColor(50,200,50));
	rule.pattern = QRegExp("\".*\"");
	rule.format = quotationFormat;
	highlightingRules.append(rule);

	functionFormat.setFontItalic(true);
	functionFormat.setForeground(QColor(100,100,200));
	rule.pattern = QRegExp("\\b[A-Za-z0-9_]+(?=\\()");
	rule.format = functionFormat;
	highlightingRules.append(rule);

	numberFormat.setForeground(QColor(50,200,50));
	rule.pattern = QRegExp("\\b\\d+\\.?d*\\b");
	rule.format = numberFormat;
	highlightingRules.append(rule);

	singleLineCommentFormat.setForeground(QColor(200,50,50));
	rule.pattern = QRegExp("#[^\n]*");
	rule.format = singleLineCommentFormat;
	highlightingRules.append(rule);

	errorFormat.setUnderlineStyle(QTextCharFormat::WaveUnderline);
	errorFormat.setUnderlineColor(QColor::fromRgb(200,0,0,255));
}

void Highlighter::highlightBlock(const QString &text)
{
	foreach (const HighlightingRule &rule, highlightingRules) {
		QRegExp expression(rule.pattern);
		int index = expression.indexIn(text);
		while (index >= 0) {
			int length = expression.matchedLength();
			setFormat(index, length, rule.format);
			index = expression.indexIn(text, index + length);
		}
	}
}
