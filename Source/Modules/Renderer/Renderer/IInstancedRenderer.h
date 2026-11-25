#pragma once

#include <vector>
#include <unordered_map>

#include "Assets/Core/GUID.h"

#include "Core/Math/Vector/Vector2.h"

namespace Eclipse
{
	class BaseRenderComponent;

	class TransformBuffer;
	class SpriteBuffer;

	namespace Assets
	{
		class Material;
		class Texture;
	}
}

namespace Eclipse::Graphics
{
	struct InstancedRenderObject
	{
		TransformBuffer* TransformBuffer;
		SpriteBuffer* SpriteBuffer;
		Assets::Material* Material;
		Assets::Texture* Sprite;

		struct InternalInstanceTransform
		{
			Math::Vector2f Position;
			Math::Vector2f Scale;
			float Rotation = 0;
		}  InstanceTransform;
	};

	class IInstancedRenderer
	{
	public:
		virtual void Init() = 0;
		virtual void AddRenderObject(TransformBuffer* aTransformBuffer, SpriteBuffer* aSpriteBuffer, Assets::Material* aMaterial, Assets::Texture* aSprite) = 0;
		virtual void ExecuteStaticRendering() = 0;
		void ClearList()
		{
			RenderComponents.clear();
		}

		std::unordered_map<Assets::GUID, std::vector<InstancedRenderObject>, Assets::GUIDHash> RenderComponents;
	};
}