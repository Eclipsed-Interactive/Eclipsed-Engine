#pragma once

#include "Renderer.Core.hpp"

namespace Eclipse::Graphics
{
	class RENDERER_API Sprite
	{
	public:
		unsigned& GetVertexBufferID();
		unsigned GetVertexBufferID() const;

		unsigned& GetIndexBufferID();
		unsigned GetIndexBufferID() const;

		unsigned& GetSpriteBufferID();
		unsigned GetSpriteBufferID() const;

	private:
		unsigned myVTXbuffer;
		unsigned myIDXbuffer;
		unsigned mySpriteBuffer;

	public:
		struct Vertex
		{
			float posX;
			float posY;

			float texCoordX;
			float texCoordY;
		};

	public:
#pragma region hard_coded_verts_indecies
		// 4 vertices
		static Vertex Verticies[4];

		// 6 indices
		static unsigned Indices[6];

#pragma endregion
	};
}