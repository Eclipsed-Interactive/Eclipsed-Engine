#include "PhysicsEngine.h"

namespace Eclipse
{
    void PhysicsEngine::ChangeBodyType(b2BodyId* aBodyID, BodyType aBodyType)
    {
        if (!aBodyID) return;
        b2Body_SetType(*aBodyID, static_cast<b2BodyType>(aBodyType));
    }

    void PhysicsEngine::SetPhysicsMaterial(b2ShapeId* aShape, const PhysMaterial& material)
    {
        b2SurfaceMaterial b2SurfaceMaterial = {};

        b2SurfaceMaterial.friction = material.friction;
        b2SurfaceMaterial.restitution = material.restitusion;

        b2Shape_SetSurfaceMaterial(*aShape, &b2SurfaceMaterial);
    }

    void PhysicsEngine::ChangeLayer(b2ShapeId* aShapeID, Layer aLayer)
    {
        b2Filter filter;
        filter.categoryBits = static_cast<uint64_t>(aLayer);

        int layerIndex = std::countr_zero(static_cast<uint32_t>(aLayer));
        filter.maskBits = myCollisionLayers[layerIndex];

        b2Shape_SetFilter(*aShapeID, filter);
    }

    void PhysicsEngine::ChangeRBLocks(b2BodyId* aBodyID, bool XLock, bool YLock, bool RotationLock)
    {
        auto ml = b2MotionLocks(XLock, YLock, RotationLock);

        b2Body_SetMotionLocks(*aBodyID, ml);
        b2Body_SetAwake(*aBodyID, true);
    }


    void PhysicsEngine::SetPosition(b2BodyId* aBodyID, const Math::Vector2f& aPosition)
    {
        b2Rot rotation = b2Body_GetRotation(*aBodyID);
        b2Body_SetTransform(*aBodyID, b2Vec2(aPosition.x, aPosition.y), rotation);
    }
    void PhysicsEngine::SetRotation(b2BodyId* aBodyID, float aRotation)
    {
        b2Vec2 postition = b2Body_GetPosition(*aBodyID);

        b2Rot rot;
        rot.c = cosf(aRotation);
        rot.s = sinf(aRotation);

        b2Body_SetTransform(*aBodyID, postition, rot);
    }

    void PhysicsEngine::SetTransform(b2BodyId* aBodyID, const Math::Vector2f& aPosition, float aRotation)
    {
        b2Rot rot;
        rot.c = cosf(aRotation);
        rot.s = sinf(aRotation);

        b2Body_SetTransform(*aBodyID, b2Vec2(aPosition.x, aPosition.y), rot);
    }

    void PhysicsEngine::SetTransformBox(b2BodyId* aBodyID, const Math::Vector2f& aPosition, float aRotation, const Math::Vector2f& aScale, const Math::Vector2f& aPivot)
    {
        SetTransform(aBodyID, aPosition, aRotation);

        b2ShapeId shapeArray;
        int capacity = 1;

        b2Body_GetShapes(*aBodyID, &shapeArray, capacity);

        float absX = std::abs(aScale.x);
        float absY = std::abs(aScale.y);

        if (absX > 0 && absY > 0)
        {
            b2Rot rot;
            rot.c = cosf(0);
            rot.s = sinf(0);

            b2Polygon polygon = b2MakeOffsetBox(absX, absY, { aPivot.x, aPivot.y }, rot);
            b2Shape_SetPolygon(shapeArray, &polygon);
        }
    }

    void PhysicsEngine::SetTransformCircle(b2BodyId* aBodyID, const Math::Vector2f& aPosition, float aRotation, float aRadius, const Math::Vector2f& aPivot)
    {
        SetTransform(aBodyID, aPosition, aRotation);

        b2ShapeId shapeArray;
        int capacity = 1;

        b2Body_GetShapes(*aBodyID, &shapeArray, capacity);

        float absRadius = std::abs(aRadius);

        if (absRadius > 0)
        {
            b2Circle circle({ aPivot.x, aPivot.y }, absRadius);
            b2Shape_SetCircle(shapeArray, &circle);
        }
    }

    void PhysicsEngine::SetTransformCapsule(b2BodyId* aBodyID, const Math::Vector2f& aPosition, float aRotation, float aRadius, float aHalfHeight, const Math::Vector2f& aPivot)
    {
        SetTransform(aBodyID, aPosition, aRotation);

        b2ShapeId shapeArray;
        int capacity = 1;

        b2Body_GetShapes(*aBodyID, &shapeArray, capacity);

        float absRadius = std::abs(aRadius);
        float absHalfHeight = std::abs(aHalfHeight);

        if (absHalfHeight > 0)
        {
            b2Capsule capsule({ aPivot.x, -absHalfHeight * 0.5f + aPivot.y }, { aPivot.x, absHalfHeight * 0.5f + aPivot.y }, absRadius);
            b2Shape_SetCapsule(shapeArray, &capsule);
        }
    }

    void PhysicsEngine::SetTransformPolygon(b2BodyId* aBodyID, const Math::Vector2f& aPosition, float aRotation, const std::vector<Math::Vector2f>& aPoints, const Math::Vector2f& aScale, const Math::Vector2f& aPivot)
    {
        SetTransform(aBodyID, aPosition, aRotation);

        b2ShapeId shapeArray;
        int capacity = 1;

        b2Body_GetShapes(*aBodyID, &shapeArray, capacity);

        if (std::abs(aScale.x) > 0 && std::abs(aScale.y) > 0)
        {
            int pointCount = std::min(static_cast<int>(aPoints.size()), 8);

            std::vector<b2Vec2> vecOfVec;
            for (auto& vec : aPoints)
            {
                vecOfVec.emplace_back(b2Vec2{ vec.x * aScale.x * 10.f, vec.y * aScale.y * 10.f });
            }

            b2Hull hull = b2ComputeHull(vecOfVec.data(), pointCount);

            if (!b2ValidateHull(&hull))
                return;

            b2Polygon polygon = b2MakePolygon(&hull, 0.0001f);

            b2Shape_SetPolygon(shapeArray, &polygon);
        }
    }
}
