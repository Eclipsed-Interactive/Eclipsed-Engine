#pragma once

#include <unordered_map>

#include "SupportedAssets.h"
#include "Core/GUID.h"

#include "Assets/Data/AssetData.h"
#include "AssetSystem.Core.hpp"

namespace Eclipse::AssetSystem
{
	class ASSETSYSTEM_API AssetCache
	{
	public:
		static AssetData* Get(GUID guid);
		static AssetData* Store(GUID guid, AssetData* data);
		static void Delete(GUID guid);

		static void MarkForReload(GUID guid);
		static void ProcessReloads();

	private:
		static std::vector<GUID> pendingReload;
		static std::unordered_map<GUID, AssetData*, GUIDHash> loadedAssets;
	};
}