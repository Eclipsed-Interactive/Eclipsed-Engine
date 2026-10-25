#pragma once

#include "Renderer.Core.hpp"

#include "Core/Math/Vector/Vector2.h"

namespace Eclipse::Graphics
{
	struct ParticleData
	{
		float LifeTime = 3.f;
		Math::Vector2f Velocity = { 0.f, 0.f };

		Math::Vector2f Position = { 0.f, 0.f };
		Math::Vector2f Scale = { 0.05f, 0.05f };
		float Rotation = 0.f;

		float Trash;
	};

	class RENDERER_API ParticleDrawer
	{
	public:
		unsigned& GetVertexBufferID();
		unsigned GetVertexBufferID() const;

		unsigned& GetBufferID();
		unsigned GetBufferID() const;

		unsigned& GetVTXShaderID();
		unsigned GetVTXShaderID() const;

		unsigned& GetGeometryShaderID();
		unsigned GetGeometryShaderID() const;

		unsigned& GetPixelShaderID();
		unsigned GetPixelShaderID() const;

		unsigned& GetProgramID();
		unsigned GetProgramID() const;

	private:
		unsigned myVTXbuffer;
		unsigned myVTXArrayBuffer;

		unsigned ProgramID;
		unsigned PixelShaderID;
		unsigned GeometryShaderID;
		unsigned VTXShaderID;
	};
}