#include "SeExprFunc.h"
#include "SeExprBuiltins.h"
#include "SeVec3d.h"
#include "SeExprNode.h"

#include <OpenImageIO/texture.h>
#include <OpenImageIO/ustring.h>

#include <iostream>
#include <cmath>

OIIO::TextureSystem* textureSystem = NULL;

class Texture : public SeExprFuncX
{
public:
	Texture()
	: SeExprFuncX(true)
	{
		if (!textureSystem)
		{
			textureSystem = OIIO::TextureSystem::create(false);
		}
	}

	~Texture()
	{
	}

	/** prep the expression by doing all type checking argument checking, etc. */
	bool prep(SeExprFuncNode* node, bool wantVec)
	{
		if (node->numChildren() != 2)
		{
			node->addError("Wrong number of arguments. Two arguments expected");
			return false;
		}

		if (!node->isStrArg(0))
		{
			node->addError("First argument must be a texture filename");
			return false;
		}


		m_texturePath = node->getStrArg(0);

		node->child(1)->prep(wantVec);

		m_texturePathIndex = OIIO::ustring(m_texturePath);

		return true;
	}

	/** evaluate the expression. the given node is where in the parse tree the evaluation is for */
	void eval(const SeExprFuncNode* node, SeVec3d& result) const
	{
		SeVec3d r;
		node->child(1)->eval(r);
		float sampleResult[4];
		OIIO::TextureOpt options;

		options.nchannels = 3;

		bool sr = textureSystem->texture(OIIO::ustring(m_texturePath), options, (float) r[0], (float) r[1], 1.0f, 0.0f, 0.0f, 1.0f, &sampleResult[0]);
		if (!sr)
		{
			std::cout << m_texturePath << std::endl;
			std::cout << sr << std::endl;
			std::cout << textureSystem->geterror() << std::endl;
		}


		result = SeVec3d(sampleResult[0], sampleResult[1], sampleResult[2]);
	}
private:

	std::string m_texturePath;

	OIIO::ustring m_texturePathIndex;

} texture;

extern "C" void SeExprPluginInitV2(SeExprFunc::Define3 define)
{
    define("map", SeExprFunc(texture,2,2), "sample from a texture");
}

