#include "AssetMetaManager.h"

#include "MetaFileRegistry.h"

namespace Eclipse
{
	std::filesystem::path AssetMetaManager::CreateMetaFile(const std::filesystem::path& aPath)
	{
		return MetaFileRegistry::CreateMetaFile(aPath);
	}

	std::filesystem::path AssetMetaManager::GetMetaFile(const std::filesystem::path& aPath)
	{
		return MetaFileRegistry::GetMetaFilePath(aPath);
	}
}