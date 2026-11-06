#pragma once

#include "Core/Math/Vector/Vector3.h"

namespace Eclipse
{
    struct SharedGPUParticlesBuffer
    {
        Math::Vector3f Gravity = { 0.f, 0.f, 0.f };
        float DeltaTime = 0.f;
        Math::Vector3f EmitterPosition = { 0.f, 0.f, 0.f };
        unsigned SpawnCount = 0;
        unsigned FrameSeed = 0;
        Math::Vector3f Padding = { 0.f, 0.f, 0.f };
    };
}