#pragma once

#include <vector>
#include <unordered_map>

#include "Core/GUID.h"

#include "Core/Math/Vector/Vector2.h"



namespace Eclipse
{
	class BaseRenderComponent;

	class TransformBuffer;
	class SpriteBuffer;

	class MaterialBuffer;
	class Texture;
}

namespace Eclipse::Graphics
{
	struct InstancedRenderObject
	{
		TransformBuffer* TransformBuffer;
		SpriteBuffer* SpriteBuffer;
		MaterialBuffer* MaterialBuffer;
		Texture* Sprite;

		Sprite::InstanceTransform InstanceTransform;
	};

	class IInstancedRenderer
	{
	public:
		virtual void Init() = 0;
		virtual void AddRenderObject(TransformBuffer* aTransformBuffer, SpriteBuffer* aSpriteBuffer, MaterialBuffer* aMaterial, Texture* aSprite, int PixelPickMaterial) = 0;
		virtual void ExecuteStaticRendering() = 0;
		virtual void ExecutePixelPickingStaticRendering() = 0;
		void ClearList()
		{
			RenderComponents.clear();
		}

		std::unordered_map<GUID, std::vector<InstancedRenderObject>, GUIDHash> RenderComponents;
	};
}