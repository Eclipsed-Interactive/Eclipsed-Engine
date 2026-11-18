#pragma once

#include <Core/Math/Vector/Vector4.h>

// Remember to add the graphics buffer enum when done with graphicsbuffer struct

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

