#pragma once

#include <stdexcept>
#include <string>
#include <vector>

#include <Core/GUID.h>

#include <AssetSystem/Artifacts/ArtifactRegistry.h>
#include <AssetSystem/Loading/AssetLoaderRegistry.h>
#include <AssetSystem/AssetCache.h>
#include "AssetSystem/ArtifactHeader.h"
#include "AssetSystem.Core.hpp"

namespace Eclipse::AssetSystem
{
	class ASSETSYSTEM_API AssetLoader
	{
	public:
		template<typename T>
		static T Load(GUID aGuid);

		template<typename T>
		static T Load(const char* aVirtualPath);
	};

	template<typename T>
	inline T AssetLoader::Load(GUID aGuid)
	{
		if (AssetData* cached = AssetCache::Get(aGuid))
			return T(cached);

		std::vector<Byte> bytes;
		if (!ArtifactRegistry::Read(aGuid, bytes) || bytes.empty())
			throw std::runtime_error("Artifact not found: " + aGuid.ToString());

		const IAssetLoader* loader = AssetLoaderRegistry::FindByMagic(T::Magic);
		if (!loader)
			throw std::runtime_error("No asset loader registered for asset: " + aGuid.ToString());

		AssetData* data = AssetCache::Store(aGuid, loader->Create());

		ArtifactReader reader(bytes);

		ArtifactHeader header;
		reader.Read(&header, sizeof(ArtifactHeader));

		loader->Load(reader, data);

		return T(data);
	}

	template<typename T>
	inline T AssetLoader::Load(const char* aVirtualPath)
	{
		GUID guid;
		if(!ArtifactRegistry::TryGetGuid(std::string(aVirtualPath), guid))
			throw std::runtime_error(std::string("No asset at path") + aVirtualPath);

		return Load<T>(guid);
	}
}