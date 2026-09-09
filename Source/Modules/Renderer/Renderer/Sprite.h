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
		static inline Vertex Verticies[]{
			{-0.5f, -0.5f, 0, 0},
			{0.5f, -0.5f, 1, 0},
			{-0.5f, 0.5f, 0, 1},
			{0.5f, 0.5f, 1, 1} };

		// 6 indices
		static inline unsigned Indices[]{
			0, 1, 2,
			2, 1, 3 };

#pragma endregion
	};
}