#pragma once

#include <filesystem>
#include <variant>

namespace Eclipse::Editor
{
    using AssetTarget = std::filesystem::path;

    using GameObjectTarget = unsigned int;

    using InspectableTarget = std::variant<
        std::monostate,
        AssetTarget,
        GameObjectTarget
    >;
}