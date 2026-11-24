#pragma once

#include "AssetData.h"
#include "AssetSystem/ShaderType.h"
namespace Eclipse::AssetSystem
{
	struct ShaderData : AssetData
	{
		ShaderType type;
		unsigned int shaderID = 0;
	};
}