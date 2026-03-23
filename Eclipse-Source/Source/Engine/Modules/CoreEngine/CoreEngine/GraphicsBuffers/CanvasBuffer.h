#pragma once

#include "CoreEngine/Math/Vector/Vector2.h"

namespace Eclipse
{
    struct CanvasBuffer
    {
        //float resolutionRatio;
        Math::Vector2f canvasScaleRelationOneDiv;
        
        float Padding[2];
    };
}