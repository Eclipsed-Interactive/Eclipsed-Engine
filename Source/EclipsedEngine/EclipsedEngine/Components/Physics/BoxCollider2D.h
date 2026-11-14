#pragma once

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

        SERIALIZED_FIELD_DEFAULT(bool, isTrigger, false);
    private:
        Math::Vector2f myHalfExtents;
        SERIALIZED_FIELD_DEFAULT_MIN_MAX(Math::Vector2f, HalfExtents, Math::Vector2f(0.5f, 0.5f), Math::Vector2f(0.f, 0.f), Math::Vector2f(FLT_MAX, FLT_MAX));

        Math::Vector2f myLastHalfExtents;
    };
}