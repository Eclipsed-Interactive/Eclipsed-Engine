#include "AssetCache.h"

#include <stdexcept>

#include "Metadata/AssetMeta.h"
#include "Core/MainSingleton.h"

namespace Eclipse::AssetSystem
{
	std::vector<GUID> AssetCache::pendingReload;
	std::unordered_map<GUID, AssetData*, GUIDHash> AssetCache::loadedAssets;

	AssetData* AssetCache::Get(GUID guid)
	{
		auto it = loadedAssets.find(guid);
		if (it != loadedAssets.end() && loadedAssets[guid] != nullptr) return loadedAssets[guid];

		return nullptr;
	}
	
	AssetData* AssetCache::Store(GUID guid, AssetData* data)
	{
		auto it = loadedAssets.find(guid);
		if (it != loadedAssets.end()) throw std::runtime_error("The guid already exists");

		loadedAssets[guid] = data;
		return data;
	}

	void AssetCache::Delete(GUID guid)
	{
		auto it = loadedAssets.find(guid);
		if (it != loadedAssets.end())
		{
			delete loadedAssets[guid];
			loadedAssets.erase(guid);
		}
	}

	void AssetCache::MarkForReload(GUID guid)
	{
		pendingReload.push_back(guid);
	}

	void AssetCache::ProcessReloads()
	{

	}
}