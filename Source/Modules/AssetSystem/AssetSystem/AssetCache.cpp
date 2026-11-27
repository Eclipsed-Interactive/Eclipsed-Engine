#include "AssetCache.h"

#include <stdexcept>

#include "Metadata/AssetMeta.h"
#include "Core/MainSingleton.h"

#include <AssetSystem/Loading/AssetLoaderRegistry.h>
#include <AssetSystem/Artifacts/ArtifactRegistry.h>
#include <AssetSystem/ArtifactHeader.h>

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
		for (GUID guid : pendingReload)
		{
			AssetData* data = Get(guid);
			if (!data) continue;

			std::vector<Byte> bytes;
			if (!ArtifactRegistry::Read(guid, bytes)) continue;

			const IAssetLoader* loader = AssetLoaderRegistry::FindByMagic(data->magic);

			ArtifactReader reader(bytes);
			ArtifactHeader header; reader.Read(&header, sizeof header);

			loader->Unload(data);
			loader->Load(reader, data);
		}

		pendingReload.clear();
	}
}