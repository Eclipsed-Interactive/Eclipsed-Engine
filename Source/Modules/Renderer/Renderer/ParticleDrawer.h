#pragma once

#include "Renderer.Core.hpp"

#include "Core/Math/Vector/Vector2.h"
#include "Core/Math/Vector/Vector4.h"

namespace Eclipse::Graphics
{
	struct ParticleData
	{
		Math::Vector2f Position = { 0.f, 0.f };
		Math::Vector2f Velocity = { 0.f, 0.f };
		Math::Vector2f Scale = { 0.005f, 0.005f };
		float Rotation = 0.f;

		float LifeTime = 3.f;
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

		unsigned& GetComputeVTXShaderID();
		unsigned GetComputeVTXShaderID() const;

		unsigned& GetComputeProgramID();
		unsigned GetComputeProgramID() const;

	private:
		unsigned myVTXbuffer;
		unsigned myVTXArrayBuffer;

		unsigned ProgramID;
		unsigned PixelShaderID;
		unsigned GeometryShaderID;
		unsigned VTXShaderID;

		unsigned ComputeProgramID;
		unsigned ComputeVTXShaderID;
	};
}