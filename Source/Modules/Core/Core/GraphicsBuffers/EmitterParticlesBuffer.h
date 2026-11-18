#pragma once

#include "Core/Math/Vector/Vector2.h"

// Remember to add the graphics buffer enum when done with graphicsbuffer struct

namespace Eclipse
{
    struct ConeEmitterDataBuffer
    {
        float pad0;
        float pad1;
        
        float EmitterWidthAngle;
        float EmitterRotation;
    };
}