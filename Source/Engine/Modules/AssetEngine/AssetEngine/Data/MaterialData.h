#pragma once

#include "AssetData.h"

#include "AssetEngine/Assets/PixelShader.h"
#include "AssetEngine/Assets/VertexShader.h"
#include "AssetEngine/Assets/Texture.h"

#include "CoreEngine/Math/Color.h"
#include "cereal/cereal.hpp"

#include <string>

namespace Eclipse
{
	struct SerializedMaterial {
		std::string vertexShaderGuid;
		std::string pixelShaderGuid;
		std::string textureGuid;

		template <class Archive>
		void serialize(Archive& ar)
		{
			ar(
				CEREAL_NVP(vertexShaderGuid),
				CEREAL_NVP(pixelShaderGuid),
				CEREAL_NVP(textureGuid)
			);
		}
	};

	struct MaterialData : public AssetData {
		VertexShader vertexShader;
		PixelShader pixelShader;
		Texture texture;

		Eclipse::Math::Color color;

		unsigned int programID = 0;
	};
}