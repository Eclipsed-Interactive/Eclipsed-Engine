#pragma once

#include <string>
#include <vector>

#include "Core/GUID.h"
#include <Core/Standard/Core/Types.hpp>
#include "AssetSystem.Core.hpp"

namespace Eclipse::AssetSystem
{
    class IArtifactProvider;

    class ASSETSYSTEM_API ArtifactRegistry final
    {
    public:
        static void Mount(IArtifactProvider* aProvider);

        static bool Contains(const GUID& aGuid);
        static bool Read(const GUID& aGuid, std::vector<Byte>& outBytes);
        static bool TryGetGuid(const std::string& aVirtualPath, GUID& outGuid);

    private:
        static std::vector<IArtifactProvider*> providers;
    };
}