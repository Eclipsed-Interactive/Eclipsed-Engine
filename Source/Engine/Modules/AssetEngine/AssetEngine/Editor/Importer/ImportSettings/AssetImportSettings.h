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
		std::string fileName;

		template <class Archive>
		void serialize(Archive& ar, std::uint32_t const version)
		{
			ar(
				CEREAL_NVP(guid),
				CEREAL_NVP(fileName)

			);

			if (version > 1)
			{
			}
		}
	};

	struct TextureAssetMetaSettings : public AssetMetaSettings
	{
		std::vector<Math::RectSizePos> spriteRects;

		template <class Archive>
		void serialize(Archive& ar, std::uint32_t const version)
		{
			AssetMetaSettings::serialize(ar, version);

			ar(
				CEREAL_NVP(spriteRects)
			);
		}
	};
}

CEREAL_CLASS_VERSION(Eclipse::AssetMetaSettings, 0)
CEREAL_CLASS_VERSION(Eclipse::TextureAssetMetaSettings, 0)