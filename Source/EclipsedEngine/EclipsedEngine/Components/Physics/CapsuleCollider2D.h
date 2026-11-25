#pragma once

#include "EclipsedEngine.Core.hpp"

#include "Collider2D.h"

#include "Core/Math/Vector/Vector2.h"
#include "ECS/ComponentManager.h"

namespace Eclipse
{
    class CapsuleCollider2D : public Collider2D
    {
        MAKE_REFLECTABLE(CapsuleCollider2D);
        REFLECTION_FLAG(COMPONENT);

        BASE_SELECTION(CapsuleCollider2D, 10)

    public:
        void EditorUpdate() override;

        void CreateCollider() override;

        void OnShapeDirty() override;

        void SetRadius(float aRadius);
        void SetHalfHeight(float aHalfHeight);

    private:
        [[SerializeField]] float Radius = 0.5f;
        [[SerializeField]] float HalfHeight = 1.f;
        [[SerializeField]] bool isTrigger = false;

        float myLastRadius;
        float myLastHalfHeight;
    };
}