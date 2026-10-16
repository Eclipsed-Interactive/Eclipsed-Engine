#pragma once

#include "Core/Math/Vector/Vector2.h"

namespace Eclipse
{
    struct SpriteBuffer
    {
        Math::Vector2f spriteScaleMultiplier = {0, 0};
        Math::Vector2f mirrored;
    };   
}