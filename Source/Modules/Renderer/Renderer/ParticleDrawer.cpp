#include "ParticleDrawer.h"

unsigned& Eclipse::Graphics::ParticleDrawer::GetVertexBufferID()
{
	return myVTXArrayBuffer;
}

unsigned Eclipse::Graphics::ParticleDrawer::GetVertexBufferID() const
{
	return myVTXArrayBuffer;
}

unsigned& Eclipse::Graphics::ParticleDrawer::GetBufferID()
{
	return myVTXbuffer;
}

unsigned Eclipse::Graphics::ParticleDrawer::GetBufferID() const
{
	return myVTXbuffer;
}

unsigned& Eclipse::Graphics::ParticleDrawer::GetVTXShaderID()
{
	return VTXShaderID;
}

unsigned Eclipse::Graphics::ParticleDrawer::GetVTXShaderID() const
{
	return VTXShaderID;
}

unsigned& Eclipse::Graphics::ParticleDrawer::GetGeometryShaderID()
{
	return GeometryShaderID;
}

unsigned Eclipse::Graphics::ParticleDrawer::GetGeometryShaderID() const
{
	return GeometryShaderID;
}

unsigned& Eclipse::Graphics::ParticleDrawer::GetPixelShaderID()
{
	return PixelShaderID;
}

unsigned Eclipse::Graphics::ParticleDrawer::GetPixelShaderID() const
{
	return PixelShaderID;
}

unsigned& Eclipse::Graphics::ParticleDrawer::GetProgramID()
{
	return ProgramID;
}

unsigned Eclipse::Graphics::ParticleDrawer::GetProgramID() const
{
	return ProgramID;
}



unsigned& Eclipse::Graphics::ParticleDrawer::GetParticleDataSSBOID()
{
	return ParticleDataSSBO;
}

unsigned Eclipse::Graphics::ParticleDrawer::GetParticleDataSSBOID() const
{
	return ParticleDataSSBO;
}

unsigned& Eclipse::Graphics::ParticleDrawer::GetFreeListSSBOID()
{
	return FreeListSSBO;
}

unsigned Eclipse::Graphics::ParticleDrawer::GetFreeListSSBOID() const
{
	return FreeListSSBO;
}




unsigned& Eclipse::Graphics::ParticleDrawer::GetProgressComputeShaderID()
{
	return ProgressComputeShaderID;
}

unsigned Eclipse::Graphics::ParticleDrawer::GetProgressComputeShaderID() const
{
	return ProgressComputeShaderID;
}

unsigned& Eclipse::Graphics::ParticleDrawer::GetSpawningComputeShaderID()
{
	return SpawningComputeShaderID;
}

unsigned Eclipse::Graphics::ParticleDrawer::GetSpawningComputeShaderID() const
{
	return SpawningComputeShaderID;
}

unsigned& Eclipse::Graphics::ParticleDrawer::GetProgressProgramID()
{
	return ProgressProgramID;
}

unsigned Eclipse::Graphics::ParticleDrawer::GetProgressProgramID() const
{
	return ProgressProgramID;
}

unsigned& Eclipse::Graphics::ParticleDrawer::GetSpawnProgramID()
{
	return SpawnProgramID;
}

unsigned Eclipse::Graphics::ParticleDrawer::GetSpawnProgramID() const
{
	return SpawnProgramID;
}