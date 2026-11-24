#pragma once

#include "Core/GUID.h"

#include <unordered_map>
#include <typeindex>
#include <filesystem>
#include <fstream>

#include "AssetSystem/MetaData/Data/IAssetMeta.h"

#include "cereal/cereal.hpp"
#include <cereal/types/memory.hpp>
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/polymorphic.hpp>
#include <cereal/types/string.hpp>

#include <memory>

#include "AssetSystem/AssetTypes.h"

#include "AssetSystem.Core.hpp"

namespace Eclipse::AssetSystem
{
	struct ASSETSYSTEM_API AssetPath
	{
	public:
		AssetPath() = default;
		AssetPath(const char* aScheme, const std::filesystem::path& aRelativePath);
		AssetPath(const char* aVirtualPath);

		void SetVirtualPath(const char* aVirtualPath);

		const std::string& Get() const;

		const std::string& Path() const;
		const std::string& Scheme() const;
		const std::string& FileName() const;

	private:
		std::string myVirtualPath;
		std::string myRelativePath;
		std::string myFileName;
		std::string myScheme;
	};
}