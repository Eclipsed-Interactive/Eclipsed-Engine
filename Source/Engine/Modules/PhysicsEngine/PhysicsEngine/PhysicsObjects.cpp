#include "PhysicsEngine.h"


namespace Eclipse
{
    void PhysicsEngine::RemoveRigidBody(b2BodyId* aBodyID)
    {
        b2DestroyBody(*aBodyID);
        *aBodyID = b2_nullBodyId;
    }

    void PhysicsEngine::RemoveCollider(b2ShapeId* aShape)
    {
        b2DestroyShape(*aShape, false);
    }

    void PhysicsEngine::CreateRigidBody(b2BodyId* aBody,
        UserData* aUserData,
        Box2DBodyType BodyType,
        bool LockRotation,
        bool LockXPos,
        bool LockYPos,
        const Math::Vector2f& aStartPosition)
    {
        b2BodyDef bodyDefine = b2DefaultBodyDef();

        b2MotionLocks motionLocks(LockXPos, LockYPos, LockRotation);
        bodyDefine.motionLocks = motionLocks;

        bodyDefine.type = static_cast<b2BodyType>(BodyType);
        bodyDefine.position = b2Vec2(aStartPosition.x, aStartPosition.y);

        bodyDefine.userData = aUserData;

        *aBody = b2CreateBody(myWorld, &bodyDefine);
    }

    void PhysicsEngine::CreateBoxCollider(b2ShapeId* aShape, const b2BodyId* aBodyID, const Math::Vector2f& aHalfExtents, Layer aLayer)
    {
        b2Polygon polygon = b2MakeBox(aHalfExtents.x, aHalfExtents.y);
        b2ShapeDef shapeDef = b2DefaultShapeDef();

        shapeDef.enableCustomFiltering = true;
        shapeDef.enableHitEvents = true;
        shapeDef.enableContactEvents = true;

        shapeDef.filter.categoryBits = static_cast<uint64_t>(aLayer);

        int layerIndex = std::countr_zero(static_cast<uint32_t>(aLayer));
        shapeDef.filter.maskBits = myCollisionLayers[layerIndex];

        *aShape = b2CreatePolygonShape(*aBodyID, &shapeDef, &polygon);
    }

    void PhysicsEngine::CreateCircleCollider(b2ShapeId* aShape, const b2BodyId* aBodyID, float radius, Layer aLayer)
    {
        b2Circle cicle({ 0, 0 }, radius);
        b2ShapeDef shapeDef = b2DefaultShapeDef();

        shapeDef.enableCustomFiltering = true;
        shapeDef.enableHitEvents = true;
        shapeDef.enableContactEvents = true;

        shapeDef.filter.categoryBits = static_cast<uint64_t>(aLayer);

        int layerIndex = std::countr_zero(static_cast<uint32_t>(aLayer));
        shapeDef.filter.maskBits = myCollisionLayers[layerIndex];

        *aShape = b2CreateCircleShape(*aBodyID, &shapeDef, &cicle);
    }

    void PhysicsEngine::CreateCapsuleCollider(b2ShapeId* aShape, const b2BodyId* aBodyID, float aHalfHeight, float aRadius, Layer aLayer)
    {
        b2Capsule capsule({ 0, -aHalfHeight * 0.5f }, { 0, aHalfHeight * 0.5f }, aRadius);
        b2ShapeDef shapeDef = b2DefaultShapeDef();

        shapeDef.enableCustomFiltering = true;
        shapeDef.enableHitEvents = true;
        shapeDef.enableContactEvents = true;

        shapeDef.filter.categoryBits = static_cast<uint64_t>(aLayer);

        int layerIndex = std::countr_zero(static_cast<uint32_t>(aLayer));
        shapeDef.filter.maskBits = myCollisionLayers[layerIndex];

        *aShape = b2CreateCapsuleShape(*aBodyID, &shapeDef, &capsule);
    }

    bool PhysicsEngine::CreatePolygonCollider(b2ShapeId* aShape, const b2BodyId* aBodyID, const std::vector<Math::Vector2f>& aPolygonPoints, Layer aLayer)
    {
        int pointCount = std::min(static_cast<int>(aPolygonPoints.size()), B2_MAX_POLYGON_VERTICES);

        std::vector<b2Vec2> vecOfVec;

        for (auto& vec : aPolygonPoints)
            vecOfVec.emplace_back(b2Vec2{ vec.x, vec.y });

        b2Hull hull = b2ComputeHull(vecOfVec.data(), pointCount);
        if (!b2ValidateHull(&hull))
            return false;

        b2Polygon polygon = b2MakePolygon(&hull, 0.0001f);
        b2ShapeDef shapeDef = b2DefaultShapeDef();

        shapeDef.enableCustomFiltering = true;
        shapeDef.enableHitEvents = true;
        shapeDef.enableContactEvents = true;

        uint64_t layer = static_cast<uint64_t>(aLayer);
        shapeDef.filter.categoryBits = layer;

        int layerIndex = std::countr_zero(layer);
        shapeDef.filter.maskBits = myCollisionLayers[layerIndex];

        *aShape = b2CreatePolygonShape(*aBodyID, &shapeDef, &polygon);

        return true;
    }

    void PhysicsEngine::DeleteBody(b2BodyId* aBody)
    {
        b2DestroyBody(*aBody);
        *aBody = b2BodyId();
    }

    void PhysicsEngine::DeleteShape(b2ShapeId* aShape)
    {
        b2DestroyShape(*aShape, false);
        *aShape = b2ShapeId();
    }
}
