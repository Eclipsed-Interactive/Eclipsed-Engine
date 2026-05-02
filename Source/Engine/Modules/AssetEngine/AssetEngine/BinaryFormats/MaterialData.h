#pragma once

#include "AssetData.h"

#include "AssetEngine/Assets/Shader/ShaderAsset.h"
#include "AssetEngine/Assets/Texture/TextureAsset.h"

#include "CoreEngine/Math/Color.h"
#include "cereal/cereal.hpp"

#include <string>

namespace Eclipse::Assets
{
	struct MaterialData : public AssetData {
		Shader shader;
		Texture texture;

		Math::Color color;

		SERIALIZE(
			MAKE_NVP(shader),
			MAKE_NVP(texture),
			MAKE_NVP(color)
		)
	};
}