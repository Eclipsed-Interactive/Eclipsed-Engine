#pragma once
#include <iostream>

#include <unordered_map>
#include <vector>
#include <string>

#include "SupportedTypes.h"

#include "AssetMetaData.h"

#include "cereal/cereal.hpp"
#include "cereal/archives/json.hpp"
#include "cereal/types/string.hpp"

#include <fstream>

#include <variant>
#include "CoreEngine/Hashing.h"

namespace Eclipse::Assets
{
	class AssetDatabase
	{
	public:
		void ImportAssets(const char* aPath, const char* aKeyPath, const int& aId = -1);

        void LoadAssetBundle(const char* aPath);

        void LoadAssets(const char* aPath, const char* aKeyPath, const int& aId);
        void LoadAsset(const char* aPath, const char* aRelativePath, const char* aKeyPath, const int& aId);

        bool MetaFileExists(const char* aPath);

        std::filesystem::path GetMetaFilePath(const char* aPath);

    public:
        const AssetMetaData& GetMetaData(size_t aGuid);
        const AssetMetaData& GetMetaData(const char* aPath);

	private:
        typedef size_t AssetGuid;
        typedef int AssetBundleFileGuid;

        std::unordered_map<AssetGuid, AssetMetaData> myAssets;
        std::unordered_map<AssetBundleFileGuid, std::vector<AssetGuid>> myBundles;
	};
}