#pragma once

#include "Core/Math/Color.h"

// Remember to add the graphics buffer enum when done with graphicsbuffer struct

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

    struct DirectionalLightBuffer
    {
        Math::Color color{ 1.f, 1.f, 1.f, 1.f };
        Math::Vector2f rotation;
        float shadowLength;
        float intensity = 1.f;
        int shadowSteps = 32;
        int padding[3];
    };

    struct AmbientLightBuffer
    {
        Math::Color color{ 1.f, 1.f, 1.f, 1.f };
        float intensity = 1.f;
        float padding[3];
    };
}