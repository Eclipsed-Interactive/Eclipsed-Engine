#pragma once

#include "Core/Math/Color.h"

namespace Eclipse
{
    struct PointLightBuffer
    {
        Math::Color color{ 1.f, 1.f, 1.f, 1.f };
        float radius = 1.f;
        float intensity = 1.f;
        float falloff = 2.f;
        float innerRadius = 0.5f;
    };

    struct SpotLightBuffer
    {
        Math::Color color{ 1.f, 1.f, 1.f, 1.f };
        float intensity = 1.f;
        float falloff = 2.f;
        float innerRadius = 0.5f;
        float cosInner = 0;
        float cosOuter = 0;
        float padding[3];
    };
}