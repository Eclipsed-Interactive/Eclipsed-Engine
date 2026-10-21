#pragma once

#include "EclipsedEngine/Components/Component.h"
#include "EclipsedEngine/Reflection/Reflection_Macros.h"
#include "EclipsedEngine/Replication/Replication_Macros.h"
namespace Eclipse
{
    class BaseRenderComponent : public Component
    {
    public:
        SERIALIZED_FIELD_DEFAULT(float, ZIndex, 0);
        float GetZIndex() override { return ZIndex; }
    };
}
