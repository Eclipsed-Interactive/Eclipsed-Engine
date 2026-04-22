#include "MetaFileRegistry.h"

#include "AssetEngine/Editor/Importer/ImportSettings/AssetImportSettings.h"

#include <fstream>

#include <cereal/cereal.hpp>
#include <cereal/archives/json.hpp>

namespace Eclipse
{
	std::filesystem::path MetaFileRegistry::GetMetaFilePath(const std::filesystem::path& aPath)
	{
		std::string filepath = aPath.generic_string();
		filepath += ".meta";

		return filepath;
	}

	std::filesystem::path MetaFileRegistry::CreateMetaFile(const std::filesystem::path& aPath)
	{
		std::filesystem::path path = GetMetaFilePath(aPath);

		std::ofstream out(path);

		return path;
	}

	bool MetaFileRegistry::MetaFileExists(const std::filesystem::path& aPath)
	{
		std::filesystem::path path = GetMetaFilePath(aPath);
		return std::filesystem::exists(path);
	}

	std::string MetaFileRegistry::GetGUID(const std::filesystem::path& aPath)
	{
		std::filesystem::path path = GetMetaFilePath(aPath);

		std::ifstream in(path);
		cereal::JSONInputArchive ar(in);

		AssetMetaSettings settings;
		ar(settings);

		return settings.guid;
	}

	std::string MetaFileRegistry::GetGUIDMeta(const std::filesystem::path& aPath)
	{
		std::ifstream in(aPath);
		cereal::JSONInputArchive ar(in);

		AssetMetaSettings settings;
		ar(settings);

		return settings.guid;
	}
}