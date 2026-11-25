#pragma once

#include "EclipsedEngine.Core.hpp"

#include "Collider2D.h"
#include "Core/Math/Vector/Vector2.h"

#include <vector>

namespace Eclipse
{
    class PolygonCollider2D : public Collider2D
    {
        MAKE_REFLECTABLE(PolygonCollider2D);
        REFLECTION_FLAG(COMPONENT);

        BASE_SELECTION(PolygonCollider2D, 10)

    public:
        void CreateCollider() override;

        void DeltaChanges() override;

        void AddPoint(const Math::Vector2f& aPoint);
        
        void OnShapeDirty() override;

    private:
        [[SerializeField]] std::vector<Math::Vector2f> myPoints;
        std::vector<Math::Vector2f> myLastPoints;

        bool CreatedShape = false;
    };
}