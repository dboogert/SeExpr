#include "SeExprFunc.h"
#include "SeExprBuiltins.h"
#include "SeVec3d.h"
#include "SeExprNode.h"

#include <OpenImageIO/texture.h>
#include <OpenImageIO/ustring.h>

#include <sstream>
#include <iostream>
#include <cmath>

//!
//! Wrapper class for OIIO texture system which is responsible for deleting the
//! TextureSystem

using namespace OpenImageIO::v1_0;

class ScopedTextureSystem
{
public:
	ScopedTextureSystem() : m_ptr(TextureSystem::create(false)) {}
	~ScopedTextureSystem()
	{
		TextureSystem::destroy(m_ptr);
		std::cout << "destorying texture system" << std::endl;
	}
	TextureSystem* operator->() { return m_ptr; }

private:
	TextureSystem* m_ptr;
};

static ScopedTextureSystem textureSystem;

class Texture : public SeExprFuncX
{
public:
	Texture()
	: SeExprFuncX(true)
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
		m_texturePathIndex = ustring(m_texturePath);

		node->child(1)->prep(wantVec);

		TextureOpt options;
		options.nchannels = 3;

		float sampleResult[4];
		bool sr = textureSystem->texture(m_texturePathIndex, options, (float) 0.0, 0.0, 1.0f, 0.0f, 0.0f, 1.0f, &sampleResult[0]);

		if (!sr)
		{
			std::stringstream ss;
			ss << "Error sampling texture:" << m_texturePath << std::endl << textureSystem->geterror();
			node->addError(ss.str());
			return false;
		}

		m_options.nchannels = 3;
		return true;
	}

	/** evaluate the expression. the given node is where in the parse tree the evaluation is for */
	void eval(const SeExprFuncNode* node, SeVec3d& result) const
	{
		SeVec3d r;
		node->child(1)->eval(r);
		float sampleResult[4];

		TextureOpt options = m_options;

		textureSystem->texture(m_texturePathIndex, options, (float) r[0], (float) r[1], 1.0f, 0.0f, 0.0f, 1.0f, &sampleResult[0]);
		result = SeVec3d(sampleResult[0], sampleResult[1], sampleResult[2]);
	}
private:
	std::string m_texturePath;
	ustring m_texturePathIndex;
	TextureOpt m_options;

} texture;

extern "C" void SeExprPluginInitV2(SeExprFunc::Define3 define)
{
    define("map", SeExprFunc(texture,2,2), "sample from a texture map");
}

