#pragma once

#include "Core/Math/Vector/Vector2.h"

namespace Eclipse
{
    struct TransformBuffer
    {
        Math::Vector2f Position;
        Math::Vector2f Scale;
        float Rotation;
        
        float Padding[3];
    };
}