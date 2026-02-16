#include "PhysicsEngine.h"

#include <box2d/box2d.h>
#include "CoreEngine/Timer.h"
#include "CoreEngine/Settings/PhysicsSettings.h"

#undef min
namespace Eclipse
{
#pragma region PhysicsHelperFunctions
    void PhysicsEngine::Physics::SetLinearVelocity(const b2BodyId* aBodyID, const Math::Vector2f& aVelocity)
    {
        if (!aBodyID) return;
        b2Vec2 box2DVec2(aVelocity.x, aVelocity.y);
        b2Body_SetLinearVelocity(*aBodyID, box2DVec2);
    }
    void PhysicsEngine::Physics::SetAngularVelocity(const b2BodyId* aBodyID, float aVelocity)
    {
        b2Body_SetAngularVelocity(*aBodyID, aVelocity);
    }

    Math::Vector2f PhysicsEngine::Physics::GetLinearVelocity(const b2BodyId* aBodyID)
    {
        b2Vec2 velocityB2 = b2Body_GetLinearVelocity(*aBodyID);
        return { velocityB2.x, velocityB2.y };
    }
    float PhysicsEngine::Physics::GetAngularVelocity(const b2BodyId* aBodyID)
    {
        float velocity = b2Body_GetAngularVelocity(*aBodyID);
        return velocity;
    }

    Math::Vector2f PhysicsEngine::Physics::GetBodyPosition(const b2BodyId* aBodyID)
    {
        b2Vec2 position = b2Body_GetPosition(*aBodyID);
        return { position.x, position.y };
    }
    float PhysicsEngine::Physics::GetBodyRotation(const b2BodyId* aBodyID)
    {
        b2Rot rotation = b2Body_GetRotation(*aBodyID);
        return b2Rot_GetAngle(rotation);
    }

#pragma endregion

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
        b2Body_SetMotionLocks(*aBodyID, b2MotionLocks(XLock, YLock, RotationLock));
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

    void PhysicsEngine::SetGravity(const Math::Vector2f& aGravity)
    {

    }

    bool CustomFilterFunction(b2ShapeId shapeIdA, b2ShapeId shapeIdB, void* context)
    {
        b2Filter filterObjA = b2Shape_GetFilter(shapeIdA);
        b2Filter filterObjB = b2Shape_GetFilter(shapeIdB);

        if (filterObjA.categoryBits | filterObjB.maskBits)
            return true;
        if (filterObjB.categoryBits | filterObjA.maskBits)
            return true;

        return false;
    }

    void LoadLayersFromJSON(std::array<uint64_t, MAX_LAYERS>& aCollisionLayers)
    {
        auto& layers = Settings::PhysicsSettings::GetPhysicsLayers();
        for (int i = 0; i < layers.size(); i++)
        {
            aCollisionLayers[i] = layers[i];
        }
    }

    void PhysicsEngine::InitWorld()
    {
        b2WorldDef worldDef;
        worldDef = b2DefaultWorldDef();
        worldDef.gravity = b2Vec2(myGravity.x, myGravity.y);

        myWorld = b2CreateWorld(&worldDef);

        b2World_SetCustomFilterCallback(myWorld, CustomFilterFunction, (void*)0);

        myHasCreatedWorld = true;
    }

    void PhysicsEngine::Init(int aSubstepCount, const Math::Vector2f& aGravity, b2DebugDraw& aDebugdraw)
    {
        LoadLayersFromJSON(myCollisionLayers);
        //PathManager::OnProjectSet += LoadLayers;

        mySubstepCount = aSubstepCount;
        myGravity = aGravity;
        myDebugDraw = std::move(aDebugdraw);

        InitWorld();
    }

    bool& PhysicsEngine::GetDebugDraw()
    {
        return myDrawDebugShapes;
    }

    bool& PhysicsEngine::GetDebugDrawShapes(DebugDrawTypes aType)
    {
        switch (aType)
        {
        case DebugDrawTypes::drawShapes: return myDebugDraw.drawShapes;
        case DebugDrawTypes::drawJoints: return myDebugDraw.drawJoints;
        case DebugDrawTypes::drawJointExtras: return myDebugDraw.drawJointExtras;
        case DebugDrawTypes::drawBounds: return myDebugDraw.drawBounds;
        case DebugDrawTypes::drawMass: return myDebugDraw.drawMass;
        case DebugDrawTypes::drawBodyNames: return myDebugDraw.drawBodyNames;
        case DebugDrawTypes::drawContacts: return myDebugDraw.drawContacts;
        case DebugDrawTypes::drawGraphColors: return myDebugDraw.drawGraphColors;
        case DebugDrawTypes::drawContactNormals: return myDebugDraw.drawContactNormals;
        case DebugDrawTypes::drawContactImpulses: return myDebugDraw.drawContactImpulses;
        case DebugDrawTypes::drawContactFeatures: return myDebugDraw.drawContactFeatures;
        case DebugDrawTypes::drawFrictionImpulses: return myDebugDraw.drawFrictionImpulses;
        case DebugDrawTypes::drawIslands: return myDebugDraw.drawIslands;

        case DebugDrawTypes::drawQueries: return myDrawQueries;
        }

        return myDrawDebugShapes;
    }

    void PhysicsEngine::LoadLayers()
    {
        LoadLayersFromJSON(myCollisionLayers);
    }

    void PhysicsEngine::CleanUp()
    {
        if (myHasCreatedWorld)
            b2DestroyWorld(myWorld);

        myHasCreatedWorld = false;
    }

    void PhysicsEngine::Update()
    {
        const float timeScale = Time::GetTimeScale();
        const float timeStep = timeScale / 800.f;

        b2World_Step(myWorld, timeStep, mySubstepCount);

        CheckCollisions();
    }
    void PhysicsEngine::DrawPhysicsObjects()
    {
        // TODO: Change to a define to not even compile debug physics if in not debug maybe potensially idk :P
        // TODO: Fix spelling of potencially.
        if (myDrawDebugShapes)
            b2World_Draw(myWorld, &myDebugDraw);
    }

    void HandleBeginContacts(b2ContactBeginTouchEvent& aEvent)
    {
        const b2ShapeId shapeIdA = aEvent.shapeIdA;
        const b2ShapeId shapeIdB = aEvent.shapeIdB;

        if (!b2Shape_IsValid(shapeIdA) || !b2Shape_IsValid(shapeIdB))
            return;

        const b2BodyId bodyIdA = b2Shape_GetBody(shapeIdA);
        const b2BodyId bodyIdB = b2Shape_GetBody(shapeIdB);

        void* userInternalDataA = b2Body_GetUserData(bodyIdA);
        void* userInternalDataB = b2Body_GetUserData(bodyIdB);

        UserData userDataA = *reinterpret_cast<UserData*>(userInternalDataA);
        UserData userDataB = *reinterpret_cast<UserData*>(userInternalDataB);

        PhysicsEngine::myBeginContactCallback(userDataA);
        PhysicsEngine::myBeginContactCallback(userDataB);
    }

    void HandleEndContacts(b2ContactEndTouchEvent& aEvent)
    {
        const b2ShapeId shapeIdA = aEvent.shapeIdA;
        const b2ShapeId shapeIdB = aEvent.shapeIdB;

        if (!b2Shape_IsValid(shapeIdA) || !b2Shape_IsValid(shapeIdB))
            return;

        const b2BodyId bodyIdA = b2Shape_GetBody(shapeIdA);
        const b2BodyId bodyIdB = b2Shape_GetBody(shapeIdB);

        void* userInternalDataA = b2Body_GetUserData(bodyIdA);
        void* userInternalDataB = b2Body_GetUserData(bodyIdB);

        UserData userDataA = *reinterpret_cast<UserData*>(userInternalDataA);
        UserData userDataB = *reinterpret_cast<UserData*>(userInternalDataB);

        PhysicsEngine::myEndContactCallback(userDataA);
        PhysicsEngine::myEndContactCallback(userDataB);
    }

    void PhysicsEngine::CheckCollisions()
    {
        b2ContactEvents contactEvents = b2World_GetContactEvents(myWorld);

        for (int i = 0; i < contactEvents.beginCount; ++i)
        {
            b2ContactBeginTouchEvent event = contactEvents.beginEvents[i];
            HandleBeginContacts(event);
        }
        // for (int i = 0; i < contactEvents.hitCount; ++i)
        // {
        //     b2ContactBeginTouchEvent event = contactEvents.beginEvents[i];
        //     HandleBeginContacts(event);
        // }
        for (int i = 0; i < contactEvents.endCount; ++i)
        {
            b2ContactEndTouchEvent event = contactEvents.endEvents[i];
            HandleEndContacts(event);
        }
    }

    float MyRayCastCallback(b2ShapeId aShape, b2Vec2 aPoint, b2Vec2 aNormal, float aFraction, void* aContext)
    {
        HitResults* myContext = reinterpret_cast<HitResults*>(aContext);

        HitResult& hitContext = myContext->results.emplace_back();

        hitContext.point = { aPoint.x, aPoint.y };
        hitContext.normal = { aNormal.x, aNormal.y };
        hitContext.fraction = aFraction;

        const b2BodyId bodyIdA = b2Shape_GetBody(aShape);
        void* userData = b2Body_GetUserData(bodyIdA);

        if (userData)
            hitContext.gameobject = reinterpret_cast<UserData*>(userData)->gameobject;

        return aFraction;
    }

    bool MyShapeOverlapCallback(b2ShapeId aShape, void* aContext)
    {
        HitResults* myContext = reinterpret_cast<HitResults*>(aContext);

        HitResult& hitContext = myContext->results.emplace_back();

        const b2BodyId bodyIdA = b2Shape_GetBody(aShape);
        void* userData = b2Body_GetUserData(bodyIdA);

        if (userData)
            hitContext.gameobject = reinterpret_cast<UserData*>(userData)->gameobject;

        return true;
    }

    bool PhysicsEngine::RayCast(const Ray& aRay, HitResults& aHitResults, float length, Layer aLayerMask)
    {
        b2QueryFilter filter = b2DefaultQueryFilter();
        filter.maskBits = static_cast<uint64_t>(aLayerMask);
        filter.categoryBits = static_cast<uint64_t>(Layer::All);

        HitResults castContext;
        b2Vec2 translation = b2MulSV(length, b2Normalize(b2Vec2(aRay.direction.x, aRay.direction.y)));
        b2World_CastRay(myWorld, b2Vec2(aRay.position.x, aRay.position.y), translation, filter, MyRayCastCallback, &castContext);

        if (!castContext.results.empty())
        {
            size_t size = sizeof(HitResult) * castContext.results.size();

            aHitResults.results.resize(castContext.results.size());
            std::memcpy(aHitResults.results.data(), castContext.results.data(), size);

            return true;
        }
        return false;
    }

    bool PhysicsEngine::OverlapBox(const Math::Vector2f& aPositon, const Math::Vector2f& aHalfExent, HitResults& aHitResults, Layer aLayerMask)
    {
        b2AABB aabb;

        Math::Vector2f lowerBoundAABB = aPositon - aHalfExent;
        Math::Vector2f upperBoundAABB = aPositon + aHalfExent;

        aabb.lowerBound = b2Vec2(lowerBoundAABB.x, lowerBoundAABB.y);
        aabb.upperBound = b2Vec2(upperBoundAABB.x, upperBoundAABB.y);

        b2QueryFilter filter = b2DefaultQueryFilter();
        filter.maskBits = static_cast<uint64_t>(aLayerMask);
        filter.categoryBits = static_cast<uint64_t>(Layer::All);

        b2World_OverlapAABB(myWorld, aabb, filter, MyShapeOverlapCallback, &aHitResults);
        if (!aHitResults.results.empty())
            return true;

        return false;
    }

    bool PhysicsEngine::OverlapSphere(const Math::Vector2f& aPositon, float aRadius, HitResults& aHitResults, Layer aLayerMask)
    {
        b2Circle circle({ aPositon.x, aPositon.y }, aRadius);
        b2ShapeProxy proxy = b2MakeProxy(&circle.center, 1, circle.radius);

        b2QueryFilter filter = b2DefaultQueryFilter();
        filter.maskBits = static_cast<uint64_t>(aLayerMask);
        filter.categoryBits = static_cast<uint64_t>(Layer::All);

        b2World_OverlapShape(myWorld, &proxy, filter, MyShapeOverlapCallback, &aHitResults);

        if (myDrawDebugShapes && myDrawQueries)
            myDebugDraw.DrawCircleFcn(b2Vec2(aPositon.x, aPositon.y), aRadius, b2HexColor::b2_colorBlack, nullptr);

        if (!aHitResults.results.empty())
            return true;

        return false;
    }
}