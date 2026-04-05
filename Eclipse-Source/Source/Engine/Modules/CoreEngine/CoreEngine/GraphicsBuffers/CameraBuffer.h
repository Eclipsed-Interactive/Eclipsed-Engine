#pragma once

#include "CoreEngine/Math/Vector/Vector2.h"

namespace Eclipse
{
    struct CameraBuffer
    {
        Math::Vector2f cameraPosition;
        Math::Vector2f cameraScale;

        float cameraRotation;
        float resolutionRatio;

        float Padding[2];
    };
}
