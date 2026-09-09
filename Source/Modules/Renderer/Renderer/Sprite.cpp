#include "Sprite.h"

namespace Eclipse::Graphics
{
	unsigned& Sprite::GetVertexBufferID()
	{
		return myVTXbuffer;
	}

	unsigned Sprite::GetVertexBufferID() const
	{
		return myVTXbuffer;
	}


	unsigned& Sprite::GetIndexBufferID()
	{
		return myIDXbuffer;
	}

	unsigned Sprite::GetIndexBufferID() const
	{
		return myIDXbuffer;
	}


	unsigned& Sprite::GetSpriteBufferID()
	{
		return mySpriteBuffer;
	}

	unsigned Sprite::GetSpriteBufferID() const
	{
		return mySpriteBuffer;
	}
}