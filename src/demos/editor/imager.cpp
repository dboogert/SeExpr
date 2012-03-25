/*
 SEEXPR SOFTWARE
 Copyright 2011 Disney Enterprises, Inc. All rights reserved

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are
 met:

 * Redistributions of source code must retain the above copyright
 notice, this list of conditions and the following disclaimer.

 * Redistributions in binary form must reproduce the above copyright
 notice, this list of conditions and the following disclaimer in
 the documentation and/or other materials provided with the
 distribution.

 * The names "Disney", "Walt Disney Pictures", "Walt Disney Animation
 Studios" or the names of its contributors may NOT be used to
 endorse or promote products derived from this software without
 specific prior written permission from Walt Disney Pictures.

 Disclaimer: THIS SOFTWARE IS PROVIDED BY WALT DISNEY PICTURES AND
 CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,
 BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
 FOR A PARTICULAR PURPOSE, NONINFRINGEMENT AND TITLE ARE DISCLAIMED.
 IN NO EVENT SHALL WALT DISNEY PICTURES, THE COPYRIGHT HOLDER OR
 CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND BASED ON ANY
 THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 */
/**
   @file imageSynth.cpp
 */
#include <map>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <SeExpression.h>
#include <fstream>

#include <QtGui>

//! Simple image synthesizer expression class to support our function grapher
class ImageSynthExpr : public SeExpression
{
public:
	//! Constructor that takes the expression to parse
	ImageSynthExpr(const std::string& expr)
	:SeExpression(expr)
	{}

	//! Simple variable that just returns its internal value
	struct Var:public SeExprScalarVarRef
	{
		Var(const double val):val(val){}
		Var(){}
		double val; // independent variable
		void eval(const SeExprVarNode* /*node*/,SeVec3d& result)
		{result[0]=val;}
	};
	//! variable map
	mutable std::map<std::string,Var> vars;

	//! resolve function that only supports one external variable 'x'
	SeExprVarRef* resolveVar(const std::string& name) const
	{
		std::map<std::string,Var>::iterator i=vars.find(name);
		if(i != vars.end()) return &i->second;
		return 0;
	}
};


double clamp(double x){return std::max(0.,std::min(255.,x));}

class QImage* MakeImage(const std::string& exprStr, int width, int height, std::vector<SeExpression::Error>& errors)
{
	QImage* qimage = new QImage(width, height, QImage::Format_ARGB32);
	if (exprStr.size() == 0)
		return qimage;

	ImageSynthExpr expr(exprStr);

	// make variables
	expr.vars["u"]=ImageSynthExpr::Var(0.);
	expr.vars["v"]=ImageSynthExpr::Var(0.);
	expr.vars["w"]=ImageSynthExpr::Var(width);
	expr.vars["h"]=ImageSynthExpr::Var(height);

	// check if expression is valid
	bool valid=expr.isValid();
	if(!valid){
		errors = expr.getErrors();
		return qimage;
	}

	// evaluate expression
	unsigned char* image = new unsigned char[width*height*4];
	double one_over_width=1./width,one_over_height=1./height;
	double& u=expr.vars["u"].val;
	double& v=expr.vars["v"].val;
	unsigned char* pixel = image;
	for(int row=0;row<height;row++){
		for(int col=0;col<width;col++){
			u=one_over_width*(col+.5);
			v=one_over_height*(row+.5);

			SeVec3d result = expr.evaluate();

			//            expr._interpreter->print();
			pixel[0] = clamp(result[0] * 256);
			pixel[1] = clamp(result[1] * 256);
			pixel[2] = clamp(result[2] * 256);
			pixel[3] = 255;
			qimage->setPixel(QPoint(row, col), pixel[0] | (pixel[1] << 8) | (pixel[2] << 16) | (pixel[3] << 24));
			pixel+=4;
		}
	}

	return qimage;
}
