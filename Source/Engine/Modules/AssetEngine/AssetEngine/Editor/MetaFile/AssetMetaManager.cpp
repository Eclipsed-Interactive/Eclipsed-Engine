#include "AssetMetaManager.h"

#include "MetaFileRegistry.h"

namespace Eclipse
{
	std::filesystem::path AssetMetaManager::CreateMetaFile(const std::filesystem::path& aPath)
	{
		return MetaFileRegistry::CreateMetaFile(aPath);
	}

	std::filesystem::path AssetMetaManager::GetMetaFilePath(const std::filesystem::path& aPath)
	{
		return MetaFileRegistry::GetMetaFilePath(aPath);
	}
}