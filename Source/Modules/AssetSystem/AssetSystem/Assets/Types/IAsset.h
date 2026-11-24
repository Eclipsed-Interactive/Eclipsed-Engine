#pragma once

#include <Core/GUID.h>

#include "AssetSystem.Core.hpp"

namespace Eclipse::AssetSystem
{
	struct TextureData;
	class ASSETSYSTEM_API Texture
	{
	public:
		Texture(AssetData* data);

	public:
		constexpr static unsigned int Magic = 'ETEX';

	private:
		GUID myGuid;
		TextureData* myData;
	};
}