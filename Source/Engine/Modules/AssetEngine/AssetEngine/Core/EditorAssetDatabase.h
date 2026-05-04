#pragma once

#include <string>
#include <filesystem>
#include <unordered_map>

#include "AssetEngine/Metadata/AssetMeta.h"
#include "AssetEngine/Core/GUID.h"
#include "AssetEngine/Core/SupportedAssets.h"

namespace Eclipse::Assets
{
    /// <summary>
    /// Editor class stores the source files and tracks changes in them.
    /// </summary>
    class AssetDatabase
    {
    public:
        void ProcessSource(const std::filesystem::path& path, const std::string& key);

        const AssetMeta& GetProcessedFile(const std::filesystem::path& path) const;
        const AssetMeta& GetProcessedFile(const GUID& guid) const;

        const std::vector<GUID>& GetGUIDFromSource(const std::string& source);

        const std::unordered_map<GUID, AssetMeta, GUIDHash>& GetSources() const;

    private:
        std::unordered_map<GUID, AssetMeta, GUIDHash> guidToAsset;
        std::unordered_map<std::filesystem::path, GUID> pathToGuid;
        std::unordered_map<AssetType, std::vector<GUID>> typeToAssets;
        std::unordered_map<std::string , std::vector<GUID>> sourceToAssets;
    };
}