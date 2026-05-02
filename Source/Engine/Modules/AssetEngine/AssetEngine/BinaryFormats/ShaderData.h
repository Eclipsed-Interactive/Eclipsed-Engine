#pragma once

#include "AssetData.h"

namespace Eclipse::Assets
{
	struct ShaderData : public AssetData {
		~ShaderData();

		unsigned int shaderProgramID;
	};
}