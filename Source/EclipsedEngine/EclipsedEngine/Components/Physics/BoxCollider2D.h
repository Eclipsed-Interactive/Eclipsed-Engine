#pragma once

#include "EclipsedEngine.Core.hpp"

#include "Collider2D.h"
#include "Core/Math/Vector/Vector2.h"

namespace Eclipse
{
    class ECL_API BoxCollider2D : public Collider2D
    {
        MAKE_REFLECTABLE(BoxCollider2D);
        REFLECTION_FLAG(COMPONENT);

        BASE_SELECTION(BoxCollider2D, 10)

    public:
        void CreateCollider() override;

        void DeltaChanges() override;

        void OnShapeDirty() override;

        void SetScale(const Math::Vector2f& aHalfExtents);

    private:
        Math::Vector2f myHalfExtents;

        [[SerializeField]]
        bool isTrigger = false;

        [[SerializeField, Range(0.f, FLT_MAX)]]
        Math::Vector2f HalfExtents = Math::Vector2f(0.5f, 0.5f);

        Math::Vector2f myLastHalfExtents;
    };
}