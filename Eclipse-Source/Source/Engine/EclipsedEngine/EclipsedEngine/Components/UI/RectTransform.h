#pragma once

#include "EntityEngine/ComponentManager.h"
#include "EclipsedEngine/Components/Component.h"



namespace Eclipse
{
    class RectTransform : public Component
    {
        COMPONENT_BASE_2(RectTransform, FLT_MAX)

    private:
        void UpdateTransforms();
        void DirtyUpdate();
    public:
        void AfterRenderUpdate() override;

        void AddParentPosition(GameObject* aParent, Math::Vector2f& aPosition) const;
        Math::Vector2f GetPosition() const;

        void AddFunctionToRunOnDirtyUpdate(const std::function<void()>& aFunction);
        
        //void RectTransform::AddParentScale(GameObject* aParent, Math::Vector2f& aPosition) const;

    public:
        SERIALIZED_FIELD_DEFAULT(Math::Vector2<float>, Position, Math::Vector2f(0, 0));
        SERIALIZED_FIELD_DEFAULT(Math::Vector2<float>, WidthHeightPX, Math::Vector2f(100, 100));

        Math::Vector2f GlobalPosition;
        
        Math::Vector2f lastPosition;
        Math::Vector2f lastWidthHeightPX;
        
        class Canvas* myCanvas;

        std::vector<std::function<void()>> myFunctionsToRunOnDirtyUpdate;

        bool myIsDirty = false;
    };
}