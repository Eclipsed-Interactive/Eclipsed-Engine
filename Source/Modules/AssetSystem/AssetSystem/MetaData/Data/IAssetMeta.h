#pragma once

#include <fstream>

namespace Eclipse::AssetSystem
{
	struct IAssetMeta
	{
		virtual ~IAssetMeta() = default;

		template <class Archive>
		void serialize(Archive& ar, const std::uint32_t version)
		{
			
		}
	};
}