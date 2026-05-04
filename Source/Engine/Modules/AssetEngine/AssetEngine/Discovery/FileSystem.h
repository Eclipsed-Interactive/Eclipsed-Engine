#pragma once

#include <filesystem>
#include <vector>
#include <cstdint>

namespace Eclipse::Assets
{
    struct FileEntry
    {
        std::filesystem::path fullPath;
        std::filesystem::path relativePath;
        uint64_t size;
        uint64_t lastWriteTime;
    };

    class FileSystem final
    {
    public:
        static std::vector<FileEntry> ScanDirectory(const std::filesystem::path& root);
        static std::vector<unsigned char> ReadFileBinary(const std::filesystem::path& path);

        static bool Exists(const std::filesystem::path& path);
        static bool IsDirectory(const std::filesystem::path& path);
    };
}