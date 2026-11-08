#pragma once

#include "Core/Math/Vector/Vector3.h"
#include "Core/Math/Vector/Vector4.h"

namespace Eclipse
{
    struct ParticleSpawnBuffer
    {
        Math::Vector3f Gravity = { 0.f, 0.f, 0.f };
        unsigned SpawnCount = 0;
        Math::Vector3f EmitterPosition = { 0.f, 0.f, 0.f };
        unsigned FrameSeed = 0;

        float MinSize;
        float MaxSize;

        float SpawnLifetimeMin;
        float SpawnLifetimeMax;

        unsigned SpawnShape = 0;

        Math::Vector3f Padding;
    };
}