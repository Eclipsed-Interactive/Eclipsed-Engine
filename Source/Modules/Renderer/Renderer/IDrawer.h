#pragma once

#include "Sprite.h"
#include "ParticleDrawer.h"
#include "TextSprite.h"

#include "Renderer.Core.hpp"

namespace Eclipse::Graphics
{
	class RENDERER_API IDrawer 
	{
	public:
		virtual void DrawSprite() = 0;
		virtual void DrawParticles(ParticleData* aParticles, int aParticleCount) = 0;

		Sprite* mySprite = nullptr;
		ParticleDrawer* myParticleDrawer = nullptr;
	};
}