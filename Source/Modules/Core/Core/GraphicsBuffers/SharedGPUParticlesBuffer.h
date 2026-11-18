#pragma once

#include "Core/Math/Vector/Vector3.h"
#include "Core/Math/Vector/Vector4.h"

// Remember to add the graphics buffer enum when done with graphicsbuffer struct

namespace Eclipse
{
    struct SharedGPUParticlesBuffer
    {
        Math::Vector3f Padding = { 0.f, 0.f, 0.f };
        float DeltaTime = 0.f;
    };
}