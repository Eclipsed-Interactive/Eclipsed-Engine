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
    typedef std::variant<AssetMetaData, TextureMetaData> MetaVariant;

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
        template<typename T>
        const T& GetMetaData(size_t aGuid);

        template<typename T>
        const T& GetMetaData(const char* aPath);

        const MetaVariant& GetMetaData(size_t aGuid);
        const MetaVariant& GetMetaData(const char* aPath);

	private:
        typedef size_t AssetGuid;
        typedef int AssetBundleFileGuid;

        std::unordered_map<AssetGuid, MetaVariant> myAssets;
        std::unordered_map<AssetBundleFileGuid, std::vector<AssetGuid>> myBundles;
	};

    template<typename T>
    const T& AssetDatabase::GetMetaData(size_t aGuid)
    {
        auto it = myAssets.find(aGuid);
        if (it == myAssets.end()) return AssetMetaData();
        {
            return std::get<T>(it->second);
        }
    }

    template<typename T>
    const T& AssetDatabase::GetMetaData(const char* aPath)
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
            return std::get<T>(it->second);
        }
    }
}