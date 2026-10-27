#pragma once

#include "ParticleDrawer.h"

#include "Renderer.Core.hpp"

namespace Eclipse::Graphics
{
	class RENDERER_API IComputer 
	{
	public:
		virtual void ComputeParticleUpdate(ParticleData* aParticles, int aParticleCount, unsigned SSBOID) = 0;
		virtual unsigned CreateNewShaderStorageBufferObject(ParticleData* aParticles, int aParticleCount) = 0;
	};
}