#pragma once

#include "IAssetMeta.h"
#include <vector>

#include "cereal/cereal.hpp"
#include "cereal/types/String.hpp"
#include "cereal/types/vector.hpp"
#include "cereal/types/polymorphic.hpp"

#include "CoreEngine/Math/RectSizePos.h"

namespace Eclipse::Assets
{
	struct TextureMeta : public IAssetMeta
	{
		std::vector<Math::RectSizePos> spriteRects;


		template <class Archive>
		void serialize(Archive& ar, const std::uint32_t version)
		{
			ar(CEREAL_NVP(spriteRects));
		}
	};
}

CEREAL_REGISTER_TYPE(Eclipse::Assets::TextureMeta)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Eclipse::Assets::IAssetMeta, Eclipse::Assets::TextureMeta)
CEREAL_CLASS_VERSION(Eclipse::Assets::TextureMeta, 2)