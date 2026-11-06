#pragma once

#include "Renderer/IComputer.h"

namespace Eclipse::Graphics::OpenGL
{
	class OpenGL_Computer : public IComputer
	{
	public:
		void ComputeParticleSpawning(unsigned ParticlesToSpawn) override;
		void ComputeParticleUpdate() override;
	};
}