#pragma once

#include "Sprite.h"
#include "TextSprite.h"

#include "Renderer.Core.hpp"

namespace Eclipse::Graphics
{
	class RENDERER_API IDrawer 
	{
	public:
		virtual void DrawSprite() = 0;

		Sprite* mySprite = nullptr;
	};
}