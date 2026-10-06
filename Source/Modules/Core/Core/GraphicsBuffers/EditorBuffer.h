#pragma once

#include <Core/Math/Vector/Vector4.h>

namespace Eclipse
{
    struct EditorBuffer
    {
        Math::Vector4f PixelPickColor;
        int PixelPicking = 1;
        int IsSceneView = 0;
        float Padding[2];
    };
}

