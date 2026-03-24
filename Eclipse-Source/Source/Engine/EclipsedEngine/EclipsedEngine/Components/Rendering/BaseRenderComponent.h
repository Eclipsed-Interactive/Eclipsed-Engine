#pragma once

#include "EclipsedEngine/Components/Component.h"

namespace Eclipse
{
    class BaseRenderComponent : public Component
    {
    public:
        SERIALIZED_FIELD_DEFAULT(float, ZIndex, 0);

        float GetZIndex() override { return ZIndex; }
    };
}
