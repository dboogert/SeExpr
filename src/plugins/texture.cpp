#include "SeExprFunc.h"
#include "SeExprBuiltins.h"
#include "SeVec3d.h"
#include "SeExprNode.h"
#include <iostream>
#include <cmath>

class Texture : public SeExprFuncX
{
public:
	Texture()
	: SeExprFuncX(true)
	{
		std::cout << "construct" << std::endl;
	}

	~Texture()
	{
	}

	/** prep the expression by doing all type checking argument checking, etc. */
	bool prep(SeExprFuncNode* node, bool wantVec)
	{
		std::cout << "prep" << std::endl;

		return SeExprFuncX::prep(node, wantVec);
	}

	/** evaluate the expression. the given node is where in the parse tree the evaluation is for */
	void eval(const SeExprFuncNode* node, SeVec3d& result) const
	{
		SeVec3d r;
		node->child(0)->eval(r);

		result =  SeVec3d(5.0 * std::cos(r[0]), 0.0, 0.0);
	}
};

extern "C" void SeExprPluginInitV2(SeExprFunc::Define3 define)
{

	std::cout << "init" << std::endl;

	Texture* t = new Texture();
    define("map", SeExprFunc(*t,1,2), "");
}

