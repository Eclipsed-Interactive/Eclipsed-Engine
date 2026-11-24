#pragma once

#include "AssetSystem/MetaData/Data/IAssetMeta.h"
#include <vector>

#include "cereal/cereal.hpp"
#include "cereal/types/String.hpp"
#include "cereal/types/vector.hpp"
#include "cereal/types/polymorphic.hpp"

namespace Eclipse::AssetSystem
{
	struct AudioMeta : public IAssetMeta
	{
		template <class Archive>
		void serialize(Archive& ar, const std::uint32_t version)
		{
			ar(cereal::base_class<IAssetMeta>(this));
		}
	};
}

CEREAL_REGISTER_TYPE_WITH_NAME(Eclipse::AssetSystem::AudioMeta, "Eclipse::Assets::AudioMeta")
CEREAL_REGISTER_POLYMORPHIC_RELATION(Eclipse::AssetSystem::IAssetMeta, Eclipse::AssetSystem::AudioMeta)
CEREAL_CLASS_VERSION(Eclipse::AssetSystem::AudioMeta, 1)