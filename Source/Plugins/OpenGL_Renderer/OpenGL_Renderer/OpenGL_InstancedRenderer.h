#pragma once

#include "Renderer/IInstancedRenderer.h"
#include <Assets/Assets/MaterialAsset.h>

namespace Eclipse
{
	class TransformBuffer;
	class SpriteBuffer;

	namespace Assets
	{
		class Material;
		class Texture;
	}
}

namespace Eclipse::Graphics::OpenGL
{
	class OpenGL_InstancedRenderer : public IInstancedRenderer
	{
	public:
		void Init() override;
		void AddRenderObject(TransformBuffer* aTransformBuffer, SpriteBuffer* aSpriteBuffer, Assets::Material* aMaterial, Assets::Texture* aSprite) override;
		void ExecuteStaticRendering() override;

		//Assets::Material myInstanceMaterial = {0};
	};
}