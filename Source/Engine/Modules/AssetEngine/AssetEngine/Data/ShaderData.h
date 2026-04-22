#pragma once

#include "AssetData.h"

namespace Eclipse
{
	struct ShaderData : public AssetData {
		unsigned int shaderProgramID;
	};

	struct VertexShaderData : public ShaderData {};
	struct PixelShaderData : public ShaderData {};
}