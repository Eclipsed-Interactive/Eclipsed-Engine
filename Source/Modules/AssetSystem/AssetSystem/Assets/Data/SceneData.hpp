#pragma once

#include "AssetData.h"
#include <string>
#include "AssetSystem.Core.hpp"

namespace Eclipse::AssetSystem
{
	struct ASSETSYSTEM_API SceneData : AssetData
	{
		std::string json;
	};
}