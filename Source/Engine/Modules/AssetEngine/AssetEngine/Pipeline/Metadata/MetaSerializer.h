#pragma once

#include <filesystem>
#include "AssetMeta.h"

namespace Eclipse::Assets
{
	class MetaSerializer
	{
	public:
		static bool MetaFileExists(const std::filesystem::path& path);
		static AssetMeta LoadOrCreateMeta(const std::filesystem::path& assetPath);

	private:
		static AssetMeta GetMetaType(const std::filesystem::path& assetPath);

		static AssetMeta CreateMeta(const std::filesystem::path& assetPath);
		static AssetMeta LoadMeta(const std::filesystem::path& assetPath);
		static void WriteMetaToFile(const std::filesystem::path& assetPath, const AssetMeta& assetMeta);

		static std::filesystem::path GetMetaPath(const std::filesystem::path& assetPath);
	};
}