#pragma once

#include "AssetData.h"

#include "AssetEngine/Assets/Shader/VertexShaderAsset.h"
#include "AssetEngine/Assets/Shader/PixelShaderAsset.h"
#include "AssetEngine/Assets/TextureAsset.h"

#include "CoreEngine/Math/Color.h"
#include "cereal/cereal.hpp"

#include <string>

namespace Eclipse::Assets
{
	struct MaterialData : public AssetData {
		VertexShader vertexShader;
		PixelShader pixelShader;
		Texture texture;

		Math::Color color;

		SERIALIZE(
			MAKE_NVP(vertexShader),
			MAKE_NVP(pixelShader),
			MAKE_NVP(texture),
			MAKE_NVP(color)
		)
	};
}