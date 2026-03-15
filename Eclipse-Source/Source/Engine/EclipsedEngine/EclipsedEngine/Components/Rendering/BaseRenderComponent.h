#pragma once

#include "EclipsedEngine/Components/Component.h"

namespace Eclipse
{
    class BaseRenderComponent : public Component
    {
    public:
        SERIALIZED_FIELD_DEFAULT(int, ZIndex, 0);

        int GetZIndex() override { return ZIndex; }
    };
}
