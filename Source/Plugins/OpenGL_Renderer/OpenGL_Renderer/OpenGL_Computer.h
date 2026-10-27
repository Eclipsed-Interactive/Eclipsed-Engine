#pragma once

#include "Renderer/IComputer.h"

namespace Eclipse::Graphics::OpenGL
{
	class OpenGL_Computer : public IComputer
	{
	public:
		void ComputeParticleUpdate(ParticleData* aParticles, int aParticleCount, unsigned SSBOID) override;
		unsigned CreateNewShaderStorageBufferObject(ParticleData* aParticles, int aParticleCount) override;
	};
}