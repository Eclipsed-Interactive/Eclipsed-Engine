#pragma once

#include "ParticleDrawer.h"

#include "Renderer.Core.hpp"

namespace Eclipse::Graphics
{
	class RENDERER_API IComputer 
	{
	public:
		virtual void ComputeParticleSpawning(unsigned ParticlesToSpawn) = 0;
		virtual void ComputeParticleUpdate() = 0;
	};
}