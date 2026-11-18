#pragma once

#include "Core/Math/Vector/Vector2.h"

// Remember to add the graphics buffer enum when done with graphicsbuffer struct

namespace Eclipse
{
    struct TransformBuffer
    {
        Math::Vector2f Position{0, 0};
        Math::Vector2f Scale{1, 1};
        float Rotation{0};
        
        float Padding[3];
    };
}