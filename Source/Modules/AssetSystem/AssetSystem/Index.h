#pragma once

#include <filesystem>
#include <string>
#include <unordered_map>

#include <Core/GUID.h>

#include "AssetSystem/AssetPath.h"

namespace Eclipse::AssetSystem
{
	class ArtifactIndex
	{
    public:
        void Add(const GUID& aGuid, const AssetPath& aPath);
        void Remove(const GUID& aGuid);

        void Clear();

        bool TryGetGuid(const std::string& aPath, GUID& outGuid) const;
        const AssetPath* TryGetPath(const GUID& aGuid) const;

    private:
        std::unordered_map<std::string, GUID> pathToGuid;
        std::unordered_map<GUID, AssetPath, GUIDHash> guidToPath;
	};
}