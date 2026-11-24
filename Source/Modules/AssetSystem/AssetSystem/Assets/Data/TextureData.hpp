#pragma once

#include "AssetData.h"
#include "Core/Math/Vector/Vector2.h"
#include "Core/Math/RectSizePos.h"
#include "AssetSystem.Core.hpp"
namespace Eclipse::AssetSystem
{
	struct ASSETSYSTEM_API TextureData : AssetData
	{
		int width = 0;
		int height = 0;
		int channels = 0;

		float ratio = 0.f;

		Math::Vector2f dimDivOne;
		Math::Vector2f sizeNormalized;

		unsigned int textureID = 0;

		std::vector<Math::RectSizePos> spriteRects;
	};
}