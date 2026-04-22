#pragma once

#include <filesystem>
#include <fstream>

namespace Eclipse
{
	template<typename AssetData>
	class IAssetLoader
	{
	public:
		virtual ~IAssetLoader() = default;

		virtual AssetData* Load(std::ifstream& in) const = 0;
	};
}