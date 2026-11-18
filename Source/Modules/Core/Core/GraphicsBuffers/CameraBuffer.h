#pragma once

#include "Core/Math/Vector/Vector2.h"

// Remember to add the graphics buffer enum when done with graphicsbuffer struct

namespace Eclipse
{
    struct CameraBuffer
    {
        Math::Vector2f cameraPosition{0.f, 0.f};
        Math::Vector2f cameraScale = {1.f, 1.f};
        float cameraRotation = 0.f;

        float resolutionRatio;

        float Padding[2];
    };
}
