#pragma once

#include <unordered_set>
#include <string>
#include <filesystem>

namespace Eclipse
{
	class MetaFileRegistry
	{
	public:
		static std::filesystem::path GetMetaFilePath(const std::filesystem::path& aPath);
		static std::filesystem::path CreateMetaFile(const std::filesystem::path& aPath);
		static bool MetaFileExists(const std::filesystem::path& aPath);

		static std::string GetGUID(const std::filesystem::path& aPath);
		static std::string GetGUIDMeta(const std::filesystem::path& aPath);
	};
}