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
        Math::Vector2f ReturnPosition = Position;

        GameObject* parent = gameObject->GetParent();
        if (parent)
            AddParentPosition(parent, ReturnPosition);
        
        return ReturnPosition;
    }

    void RectTransform::EarlyUpdate()
    {
    }
}
