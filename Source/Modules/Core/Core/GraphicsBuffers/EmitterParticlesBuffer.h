#pragma once

#include "Core/Math/Vector/Vector2.h"

namespace Eclipse
{
    struct ConeEmitterDataBuffer
    {
        float MinSpeed;
        float MaxSpeed;
        
        float EmitterWidthAngle;
        float EmitterRotation;
    };
}