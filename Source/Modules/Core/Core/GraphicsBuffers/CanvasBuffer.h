#pragma once

#include "Core/Math/Vector/Vector2.h"

// Remember to add the graphics buffer enum when done with graphicsbuffer struct

namespace Eclipse
{
    struct CanvasBuffer
    {
        //float resolutionRatio;
        Math::Vector2f canvasScaleRelationOneDiv;
        Math::Vector2f canvasPositionOffset;
    };
}