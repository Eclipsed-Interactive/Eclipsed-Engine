#pragma once

#include <Core/GUID.h>
#include "Core/Math/Vector/Vector2.h"
#include "Core/Math/RectSizePos.h"

#include "AssetSystem.Core.hpp"

namespace Eclipse
{
	namespace AssetSystem
	{
		struct TextureData;
		struct AssetData;
	}

	class ASSETSYSTEM_API Texture
	{
	public:
		Texture() = default;
		Texture(AssetSystem::AssetData* data);

	public:
		constexpr static unsigned int Magic = 'ETEX';

	public:
		[[nodiscard]] Math::Vector2f GetDimDivOne() const;
		[[nodiscard]] Math::Vector2f GetSizeNormalized() const;

		[[nodiscard]] int GetHeight() const;
		[[nodiscard]] int GetWidth() const;
		[[nodiscard]] int GetChannels() const;

		[[nodiscard]] const std::vector<Math::RectSizePos>* GetSpriteRects() const;
		[[nodiscard]] float GetAspectRatio() const;
		[[nodiscard]] Math::Vector2f GetTextureSizeNormilized() const;


	public:
		[[nodiscard]] bool IsReady() const;
		void Bind(unsigned int slot = 0) const;

	private:
		GUID myGuid;
		AssetSystem::TextureData* myData = nullptr;
	};
}