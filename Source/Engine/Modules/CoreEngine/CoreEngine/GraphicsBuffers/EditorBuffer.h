#pragma once

#include <CoreEngine/Math/Vector/Vector4.h>

namespace Eclipse
{
    struct EditorBuffer
    {
        Math::Vector4f PixelPickColor;
        int notOverideColor;
        float Padding[3];
    };
}

