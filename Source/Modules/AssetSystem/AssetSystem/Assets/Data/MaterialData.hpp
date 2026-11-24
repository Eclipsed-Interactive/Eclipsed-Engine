#pragma once

#include "AssetData.h"
#include "Core/Math/Color.h"

#include "AssetSystem/Assets/Types/Texture.h"
#include "AssetSystem/Assets/Types/Shader.h"
#include "AssetSystem.Core.hpp"

namespace Eclipse::AssetSystem
{
	struct ASSETSYSTEM_API MaterialData : AssetData
	{
		Math::Color color;
		
		Texture texture;

		Shader ps;
		Shader vs;

		unsigned int programID = 0;
	};
}