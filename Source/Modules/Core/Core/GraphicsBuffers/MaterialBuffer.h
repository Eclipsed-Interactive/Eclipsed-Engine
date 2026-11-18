#pragma once
#include "Core/Math/Vector/Vector4.h"
#include "Core/Math/Color.h"

// Remember to add the graphics buffer enum when done with graphicsbuffer struct

namespace Eclipse
{
    struct MaterialBuffer
    {
        Math::Color color;
        Math::Vector4f spriteRect;
    };
}
