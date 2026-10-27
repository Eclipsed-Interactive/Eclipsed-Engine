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



unsigned& Eclipse::Graphics::ParticleDrawer::GetComputeVTXShaderID()
{
	return ComputeVTXShaderID;
}

unsigned Eclipse::Graphics::ParticleDrawer::GetComputeVTXShaderID() const
{
	return ComputeVTXShaderID;
}

unsigned& Eclipse::Graphics::ParticleDrawer::GetComputeProgramID()
{
	return ComputeProgramID;
}

unsigned Eclipse::Graphics::ParticleDrawer::GetComputeProgramID() const
{
	return ComputeVTXShaderID;
}