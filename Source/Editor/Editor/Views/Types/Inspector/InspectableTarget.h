#pragma once

#include <filesystem>
#include <variant>

#include "ECS/Types.h"

namespace Eclipse::Editor
{
    using AssetTarget = std::filesystem::path;

    using GameObjectTarget = ::Eclipse::GameObjectID;

    using InspectableTarget = std::variant<
        std::monostate,
        AssetTarget,
        GameObjectTarget
    >;
}