#pragma once

#include "Core/Math/Color.h"

namespace Eclipse
{
    struct LightBuffer
    {
        Math::Color color{ 1.f, 1.f, 1.f, 1.f };
        float intensity = 1.f;
        float falloff = 2.f;
        float innerRadius = 0.5f;
        float Padding[1];
    };
}