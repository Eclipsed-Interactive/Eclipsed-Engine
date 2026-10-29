#pragma once

#include <string>
#include <filesystem>
#include <unordered_map>

#include "Assets/Metadata/AssetMeta.h"
#include "Assets/Core/GUID.h"

#include "Core/Files/FileWatcher.h"

#include "Assets.Core.hpp"

namespace Eclipse::Assets
{
    /// <summary>
    /// Editor class stores the source files and tracks changes in them.
    /// </summary>
    class ASSET_API AssetDatabase
    {
    public:
        AssetDatabase() = default;

        AssetDatabase(const AssetDatabase&) = delete;
        AssetDatabase& operator=(const AssetDatabase&) = delete;

        AssetDatabase(AssetDatabase&&) noexcept = default;
        AssetDatabase& operator=(AssetDatabase&&) noexcept = default;

    public:
        void Reload();

        void ProcessSource(const std::filesystem::path& path, const std::string& key);
        void ProcessBundle(const std::filesystem::path& path);

        const AssetMeta& ProcessFile(const std::filesystem::path& path, const std::filesystem::path& root);

        const AssetMeta& GetProcessedFile(const std::filesystem::path& path) const;
        const AssetMeta& GetProcessedFile(const GUID& guid) const;
        const std::vector<GUID>& GetGUIDsFromAssetType(AssetType type) const;
        AssetMeta& GetMetaFromMetaPath(const std::filesystem::path& path);
        AssetMeta& GetProcessedFile(const GUID& guid);

        void RenameFile(const std::filesystem::path& oldPath, const std::filesystem::path& newPath);


        GUID GetGUIDFromFullPath(const std::filesystem::path& fullpath);

        const std::vector<GUID>& GetGUIDFromSource(const std::string& source);

        const std::unordered_map<GUID, AssetMeta, GUIDHash>& GetSources() const;

    private:
        std::unordered_map<GUID, AssetMeta, GUIDHash> guidToAsset;
        std::unordered_map<std::filesystem::path, GUID> pathToGuid;
        std::unordered_map<std::filesystem::path, GUID> fullpathToGuid;
        std::unordered_map<AssetType, std::vector<GUID>> typeToAssets;
        std::unordered_map<std::string , std::vector<GUID>> sourceToAssets;
        std::unordered_map<std::string, std::string> sourcePathToKey;
        std::unordered_map<std::string, std::string> sourceToKey;
    };
}