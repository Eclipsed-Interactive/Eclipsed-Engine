#pragma once

#include "Core/Math/Vector/Vector2.h"

namespace Eclipse
{
    struct SharedGPUParticlesBuffer
    {
        Math::Vector2f Gravity = { 0.f, 0.f };
        float DeltaTime = 0.f;
        float Trash = 0.f;
    };
}