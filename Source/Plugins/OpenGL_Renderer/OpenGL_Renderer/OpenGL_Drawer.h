#pragma once

#include "Renderer/IDrawer.h"

namespace Eclipse::Graphics::OpenGL
{
	class OpenGL_Drawer : public IDrawer
	{
	public:
		void DrawSprite() override;
		//void DrawTextSprite() override;
		//void DrawSprite(SpriteRenderer2D* spriteRend, Transform2D* transform) override;
	};
}