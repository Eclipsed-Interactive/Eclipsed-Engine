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
		void AddRenderObject(TransformBuffer* aTransformBuffer, SpriteBuffer* aSpriteBuffer, MaterialBuffer* aMaterial, Texture* aSprite) override;
		void ExecuteStaticRendering() override;

		//Assets::Material myInstanceMaterial = {0};
	};
}