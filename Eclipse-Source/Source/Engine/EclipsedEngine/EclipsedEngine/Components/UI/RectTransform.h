#pragma once

#include "EntityEngine/ComponentManager.h"
#include "EclipsedEngine/Components/Component.h"



namespace Eclipse
{
    class RectTransform : public Component
    {
        COMPONENT_BASE_2(RectTransform, FLT_MAX)

    public:
        void EarlyUpdate() override;
        
        void AddParentPosition(GameObject* aParent, Math::Vector2f& aPosition) const;
        Math::Vector2f GetPosition() const;
        
        //void RectTransform::AddParentScale(GameObject* aParent, Math::Vector2f& aPosition) const;

    public:
        SERIALIZED_FIELD_DEFAULT(Math::Vector2<float>, Position, Math::Vector2f(0, 0));
        SERIALIZED_FIELD_DEFAULT(Math::Vector2<float>, WidthHeightPX, Math::Vector2f(100, 100));

        class Canvas* myCanvas;
    };
}