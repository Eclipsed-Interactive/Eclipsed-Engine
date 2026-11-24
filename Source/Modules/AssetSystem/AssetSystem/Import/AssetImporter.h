#pragma once

#include "IAssetImporter.h"
#include "ImporterEntry.h"

#include <filesystem>

#include "AssetSystem.Core.hpp"

namespace Eclipse::AssetSystem
{
	struct SourceMount;
	struct AssetMeta;
	class ASSETSYSTEM_API AssetImporterd
	{
	public:
		static void ImportAll();
		static void Import(const SourceMount& aMount, const AssetMeta* aMeta);
	};
}