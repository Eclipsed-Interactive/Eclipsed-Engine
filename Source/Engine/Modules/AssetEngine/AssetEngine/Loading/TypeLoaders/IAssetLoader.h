#pragma once

#include "AssetEngine/Pipeline/Metadata/AssetMeta.h"
#include <string>

namespace Eclipse::Assets
{
	class IAssetLoader
	{
	public:
		virtual ~IAssetLoader() = default;

		virtual void Load(const AssetMeta& meta) = 0;
	};
}