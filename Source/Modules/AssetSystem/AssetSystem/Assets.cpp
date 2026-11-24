#include "Assets.h"

#include "SourceRegistry.h"

namespace Eclipse::Assets
{
	bool TryGetGuid(const std::filesystem::path& pathToAsset, GUID& outGuid)
	{
		return AssetSystem::SourceRegistry::TryGetGuid(pathToAsset, outGuid);
	}

	ASSETSYSTEM_API bool TryGetFull(GUID aGuid, std::filesystem::path& outPath)
	{
		return AssetSystem::SourceRegistry::TryGetFullPath(aGuid, outPath);
	}
}