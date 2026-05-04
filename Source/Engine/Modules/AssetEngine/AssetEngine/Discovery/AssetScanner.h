#pragma once

#include <filesystem>
#include <vector>

namespace Eclipse::Assets
{
    struct AssetCandidate
    {
        std::filesystem::path fullPath;
        std::filesystem::path relativePath;
        std::filesystem::path rootPath;
    };

    class AssetScanner final
    {
    public:
        static std::vector<AssetCandidate> Scan(const std::filesystem::path& root);

    private:
        static bool IsValidAssetFile(const std::filesystem::path& path);
    };
}