#include "RectTransform.h"

namespace Eclipse
{
    void RectTransform::AddParentPosition(GameObject* aParent, Math::Vector2f& aPosition) const
    {
        RectTransform* parentTransform = aParent->GetComponent<RectTransform>();
        if (!parentTransform)
            return;

        aPosition += parentTransform->Position;

        GameObject* parent = aParent->GetParent();
        if (parent && parent->transform)
            AddParentPosition(parent, aPosition);
    }

    Math::Vector2f RectTransform::GetPosition() const
    {
        return GlobalPosition;
    }

    Math::Vector2f RectTransform::GetLocalPosition()
    {
        return Position;
    }

    void RectTransform::SetPosition(const Math::Vector2f& aPosition)
    {
        Position = aPosition;
    }

    void RectTransform::AddFunctionToRunOnDirtyUpdate(const std::function<void()>& aFunction)
    {
        myFunctionsToRunOnDirtyUpdate.push_back(aFunction);
    }

    void RectTransform::EditorUpdate()
    {
        if (Position->x != lastPosition.x || Position->y != lastPosition.y)
        {
            lastPosition = Position;
            myIsDirty = true;
        }
        if (WidthHeightPX->x != lastWidthHeightPX.x || WidthHeightPX->y != lastWidthHeightPX.y)
        {
            lastWidthHeightPX = WidthHeightPX;
            myIsDirty = true;
        }

        if (myIsDirty)
        {
            DirtyUpdate();
        }
    }

    void RectTransform::UpdateTransforms()
    {
        GlobalPosition = Position;

        GameObject* parent = gameObject->GetParent();
        if (parent)
            AddParentPosition(parent, GlobalPosition);
    }

    void RectTransform::DirtyUpdate()
    {
        UpdateTransforms();

        for (auto& func : myFunctionsToRunOnDirtyUpdate)
            func();

        for (const auto& child : gameObject->GetChildren())
        {
            if (RectTransform* rectTransform = child->GetComponent<RectTransform>())
                rectTransform->DirtyUpdate();
        }
    }
}
