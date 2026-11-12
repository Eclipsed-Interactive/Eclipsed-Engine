#include "TextSprite.h"

namespace Eclipse::Graphics
{
	TextSprite::Vertex TextSprite::Vertices[4]{
		{0.f, 0.f, 0, 0},
		{1.f, 0.f, 1, 0},
		{0.f, 1.f, 0, 1},
		{1.f, 1.f, 1, 1} };

	unsigned TextSprite::Indices[6]{
		0, 1, 2,
		2, 1, 3 };


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