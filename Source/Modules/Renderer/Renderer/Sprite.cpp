#include "Sprite.h"

namespace Eclipse::Graphics
{
	Sprite::Vertex Sprite::Verticies[4]{
		{-0.5f, -0.5f, 0, 0},
		{0.5f, -0.5f, 1, 0},
		{-0.5f, 0.5f, 0, 1},
		{0.5f, 0.5f, 1, 1} };

	unsigned Sprite::Indices[6]{
		0, 1, 2,
		2, 1, 3 };


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