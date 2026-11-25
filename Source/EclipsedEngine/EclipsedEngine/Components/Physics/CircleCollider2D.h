#pragma once

#include "EclipsedEngine.Core.hpp"

#include "Collider2D.h"

#include "Core/Math/Vector/Vector2.h"

namespace Eclipse
{
    class CircleCollider2D : public Collider2D
    {
        MAKE_REFLECTABLE(CircleCollider2D);
        REFLECTION_FLAG(COMPONENT);

        BASE_SELECTION(CircleCollider2D, 10)

    public:
        void CreateCollider() override;

        void DeltaChanges() override;

        void OnShapeDirty()override;

        void SetRadius(float aRadius);

    private:
        float LastRadius = 0.f;
        float myInternalRadius = 1.f;

        [[SerializeField]] float Radius = 1.f;
        [[SerializeField]] bool isTrigger = false;
    };
}