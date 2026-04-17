#pragma once

#include "Collider2D.h"

#include "CoreEngine/Math/Vector/Vector2.h"
#include "EntityEngine/ComponentManager.h"

namespace Eclipse
{
    class CapsuleCollider2D : public Collider2D
    {
        BASE_SELECTION(CapsuleCollider2D, 10)

    public:
        void EditorUpdate() override;

        void CreateCollider() override;

        void OnTransformDirty();

        void SetRadius(float aRadius);
        void SetHalfHeight(float aHalfHeight);

    private:
        SERIALIZED_FIELD_DEFAULT(float, Radius, 0.5f);
        SERIALIZED_FIELD_DEFAULT(float, HalfHeight, 1.f);

        float myLastRadius;
        float myLastHalfHeight;
    };
}