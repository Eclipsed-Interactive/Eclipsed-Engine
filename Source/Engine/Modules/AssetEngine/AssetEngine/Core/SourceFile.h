#pragma once

#include <filesystem>
#include "GUID.h"

namespace Eclipse::Assets
{
    struct SourceFile
    {
        GUID assetId;
        std::filesystem::path fullPath;
        std::filesystem::path relativePath;
    };
}