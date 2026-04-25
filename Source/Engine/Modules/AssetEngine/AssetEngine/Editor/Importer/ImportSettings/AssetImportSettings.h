#pragma once

#include <string>
#include "cereal/cereal.hpp"
#include "cereal/types/String.hpp"
#include "cereal/types/vector.hpp"

#include "CoreEngine/Math/RectSizePos.h"
#include <vector>

namespace Eclipse
{
	struct AssetMetaSettings
	{
		size_t guid;
		std::string fileName = "";

		template <class Archive>
		void serialize(Archive& ar)
		{
			ar(
				CEREAL_NVP(guid),
				CEREAL_NVP(fileName)
			);
		}
	};

	struct TextureAssetMetaSettings : public AssetMetaSettings
	{
		std::vector<Math::RectSizePos> spriteRects;

		template <class Archive>
		void serialize(Archive& ar)
		{
			AssetMetaSettings::serialize(ar);

			ar(
				CEREAL_NVP(spriteRects)
			);
		}
	};
}