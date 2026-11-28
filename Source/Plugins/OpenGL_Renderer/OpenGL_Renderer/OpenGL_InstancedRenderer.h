#pragma once

#include "Renderer/IInstancedRenderer.h"


namespace Eclipse
{
	class TransformBuffer;
	class SpriteBuffer;

	class MaterialBuffer;
	class Texture;
}

namespace Eclipse::Graphics::OpenGL
{
	class OpenGL_InstancedRenderer : public IInstancedRenderer
	{
	public:
		void Init() override;
		void AddRenderObject(TransformBuffer* aTransformBuffer, SpriteBuffer* aSpriteBuffer, MaterialBuffer* aMaterial, Texture* aSprite, int PixelPickMaterial) override;
		void ExecuteStaticRendering() override;
		void ExecutePixelPickingStaticRendering() override;

		//Assets::Material myInstanceMaterial = {0};
	};
}