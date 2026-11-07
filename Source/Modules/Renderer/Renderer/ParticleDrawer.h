#pragma once

#include "Renderer.Core.hpp"

#include "Core/Math/Color.h"
#include "Core/Math/Vector/Vector2.h"
#include "Core/Math/Vector/Vector3.h"
#include "Core/Math/Vector/Vector4.h"

namespace Eclipse::Graphics
{
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

		unsigned& GetParticleDataSSBOID();
		unsigned GetParticleDataSSBOID() const;

		unsigned& GetFreeListSSBOID();
		unsigned GetFreeListSSBOID() const;

		unsigned& GetProgressComputeShaderID();
		unsigned GetProgressComputeShaderID() const;

		unsigned& GetSpawningComputeShaderID();
		unsigned GetSpawningComputeShaderID() const;

		unsigned& GetDestroyAllComputeShaderID();
		unsigned GetDestroyAllComputeShaderID() const;

		unsigned& GetProgressProgramID();
		unsigned GetProgressProgramID() const;

		unsigned& GetSpawnProgramID();
		unsigned GetSpawnProgramID() const;

		unsigned& GetDestroyAllProgramID();
		unsigned GetDestroyAllProgramID() const;

	private:
		unsigned myVTXbuffer;
		unsigned myVTXArrayBuffer;

		unsigned ProgramID;
		unsigned PixelShaderID;
		unsigned GeometryShaderID;
		unsigned VTXShaderID;


		unsigned ProgressProgramID;
		unsigned SpawnProgramID;
		unsigned DestroyAllProgramID;

		unsigned ProgressComputeShaderID;
		unsigned SpawningComputeShaderID;
		unsigned DestroyAllComputeShaderID;

		unsigned ParticleDataSSBO;
		unsigned FreeListSSBO;
	};
}