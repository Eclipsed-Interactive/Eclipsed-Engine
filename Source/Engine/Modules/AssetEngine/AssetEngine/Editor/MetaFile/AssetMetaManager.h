#pragma once

#include <unordered_set>
#include <string>
#include <filesystem>

namespace Eclipse
{
	class AssetMetaManager
	{
	public:
		static std::filesystem::path CreateMetaFile(const std::filesystem::path& aPath);
		static std::filesystem::path GetMetaFilePath(const std::filesystem::path& aPath);
	};
}