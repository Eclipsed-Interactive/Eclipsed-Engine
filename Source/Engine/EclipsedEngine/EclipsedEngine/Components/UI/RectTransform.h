#pragma once

#include "EntityEngine/ComponentManager.h"
#include "EclipsedEngine/Components/Component.h"



namespace Eclipse
{
    class RectTransform : public Component
    {
        COMPONENT_BASE_2(RectTransform, UINT_MAX * 0.5f)

    private:
        void UpdateTransforms();
        void DirtyUpdate();

        void AddParentPosition(GameObject* aParent, Math::Vector2f& aPosition) const;
        
    public:
        void EditorUpdate() override;

        
        Math::Vector2f GetPosition() const;
        Math::Vector2f GetLocalPosition();
        
        void SetPosition(const Math::Vector2f& aPosition);

        void AddFunctionToRunOnDirtyUpdate(const std::function<void()>& aFunction);
        
        //void RectTransform::AddParentScale(GameObject* aParent, Math::Vector2f& aPosition) const;

    public:
        SERIALIZED_FIELD_DEFAULT(Math::Vector2<float>, Position, Math::Vector2f(0, 0));
        SERIALIZED_FIELD_DEFAULT(Math::Vector2<float>, WidthHeightPX, Math::Vector2f(100, 100));
        
        SERIALIZED_FIELD_DEFAULT(bool, ScaleWithCanvasX, false);
        SERIALIZED_FIELD_DEFAULT(bool, ScaleWithCanvasY, false);

        SERIALIZED_FIELD_DEFAULT(bool, AlignLeft, false);
        SERIALIZED_FIELD_DEFAULT(bool, AlignTop, false);
        SERIALIZED_FIELD_DEFAULT(bool, AlignRight, false);
        SERIALIZED_FIELD_DEFAULT(bool, AlignBottom, false);

        Math::Vector2f GlobalPosition;
        
        Math::Vector2f lastPosition;
        Math::Vector2f lastWidthHeightPX;
        
        class Canvas* myCanvas;

        std::vector<std::function<void()>> myFunctionsToRunOnDirtyUpdate;

        bool myIsDirty = false;
    };
}