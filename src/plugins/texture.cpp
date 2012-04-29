#include "SeExprFunc.h"
#include "SeExprBuiltins.h"
#include "SeVec3d.h"

class Texture : public SeExprFuncX
{
public:
	Texture()
	: SeExprFuncX(true)
	{
	}

	~Texture()
	{
	}

	/** prep the expression by doing all type checking argument checking, etc. */
	bool prep(SeExprFuncNode* node, bool wantVec)
	{
		bool noErrors = true;
		return noErrors;
	}

	/** evaluate the expression. the given node is where in the parse tree the evaluation is for */
	void eval(const SeExprFuncNode* node, SeVec3d& result) const
	{
		result =  SeVec3d(1.0, 0.0, 0.0);
	}
};

extern "C" void SeExprPluginInit(SeExprFunc::Define define)
{
	Texture t;
    define("map", t);
}

