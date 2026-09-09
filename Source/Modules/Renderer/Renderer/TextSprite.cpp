#include "TextSprite.h"

namespace Eclipse::Graphics
{
	unsigned& TextSprite::GetVertexBufferID()
	{
		return myVTXbuffer;
	}

	unsigned TextSprite::GetVertexBufferID() const
	{
		return myVTXbuffer;
	}


	unsigned& TextSprite::GetIndexBufferID()
	{
		return myIDXbuffer;
	}

	unsigned TextSprite::GetIndexBufferID() const
	{
		return myIDXbuffer;
	}


	unsigned& TextSprite::GetSpriteBufferID()
	{
		return mySpriteBuffer;
	}

	unsigned TextSprite::GetSpriteBufferID() const
	{
		return mySpriteBuffer;
	}
}