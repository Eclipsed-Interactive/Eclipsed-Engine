#pragma once

#include "Core/Math/Vector/Vector2.h"

// Remember to add the graphics buffer enum when done with graphicsbuffer struct

namespace Eclipse
{
    struct SpriteBuffer
    {
        Math::Vector2f spriteScaleMultiplier = { 0, 0 };
        Math::Vector2f mirrored = { 1, 1 };
    };   
}