#include "PhysicsEngine.h"

#include <box2d/box2d.h>
#include "CoreEngine/Timer.h"
#include "CoreEngine/Settings/PhysicsSettings.h"

#undef min
namespace Eclipse
{
    void PhysicsEngine::SetGravity(const Math::Vector2f& aGravity)
    {
        b2World_SetGravity(myWorld, b2Vec2(aGravity.x, aGravity.y));
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
        const float deltaTime = Time::GetDeltaTime();

        b2World_Step(myWorld, deltaTime, mySubstepCount);

        CheckCollisions();
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
}