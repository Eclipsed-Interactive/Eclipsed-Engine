#include "AssetDatabase.h"

#include <iostream>
#include <filesystem>

#include "AssetEngine/AssetDatabase.h"
#include "CoreEngine/MainSingleton.h"
#include "CoreEngine/Hashing.h"

namespace Eclipse::Assets
{
	void AssetDatabase::ImportAssets(const char* aPath, const char* aKeyPath, const int& aId)
	{
		if (std::filesystem::is_directory(aPath))
			LoadAssets(aPath, aKeyPath, aId);
	}

	void AssetDatabase::LoadAssets(const char* aPath, const char* aKeyPath, const int& aId)
	{
		for (auto file : std::filesystem::recursive_directory_iterator(aPath)) {
			if (file.is_directory()) continue;
			if (file.path().extension() == ".meta") continue;

			auto path = file.path().generic_string();
			auto relativePath = std::filesystem::relative(path, aPath).generic_string();
			LoadAsset(path.c_str(), relativePath.c_str(), aKeyPath, aId);
		}
	}

	void AssetDatabase::LoadAsset(const char* aPath, const char* aRelativePath, const char* aKeyPath, const int& aId)
	{
		if (MetaFileExists(aPath))
		{ // Load the existing meta file.
			AssetMetaData metaData(aPath);

			myBundles[aId].push_back(metaData.guid);
			myAssets[metaData.guid] = metaData;
		}
		else
		{ // Create a new meta file and load it.
			std::string relativePath = aKeyPath;
			relativePath += aRelativePath;

			const size_t guid = Hashing::fnv1a(relativePath.data(), relativePath.size());
			const std::string fileName = std::filesystem::path(aPath).filename().stem().string();
			const std::string filePath = aPath;
			const std::string fileExtension = std::filesystem::path(aPath).extension().string();
			const AssetType type = GetAssetTypeFromExtension(fileExtension);

			AssetMetaData metaData;
			metaData.fileExtension = fileExtension;
			metaData.fileName = fileName;
			metaData.guid = guid;
			metaData.type = type;
			metaData.filePath = aPath;

			myBundles[aId].push_back(guid);
			myAssets[guid] = metaData;
		}
	}

	bool AssetDatabase::MetaFileExists(const char* aPath)
	{
		return std::filesystem::exists(GetMetaFilePath(aPath));
	}

	std::filesystem::path AssetDatabase::GetMetaFilePath(const char* aPath)
	{
		std::string path = aPath;
		path += ".meta";

		return path;
	}

	const MetaVariant& AssetDatabase::GetMetaData(size_t aGuid)
	{
		auto it = myAssets.find(aGuid);
		if (it == myAssets.end()) return AssetMetaData();
		{
			return it->second;
		}
	}
	
	const MetaVariant& AssetDatabase::GetMetaData(const char* aPath)
	{
		size_t guid = 0;
		if (std::filesystem::exists(aPath))
		{
			guid = AssetMetaData(aPath).guid;
		}
		else
		{
			guid = Hashing::fnv1a(aPath, strlen(aPath));
		}

		auto it = myAssets.find(guid);
		if (it == myAssets.end()) return AssetMetaData();
		{
			return it->second;
		}
	}
}