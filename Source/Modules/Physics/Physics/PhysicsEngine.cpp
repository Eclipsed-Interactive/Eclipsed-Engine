#include "PhysicsEngine.h"

#include <box2d/box2d.h>
#include "Core/Timer.h"
#include "Core/Settings/PhysicsSettings.h"

#undef min
namespace Eclipse
{
    std::function<void(UserData&, UserData&)> PhysicsEngine::myBeginContactCallback;
    std::function<void(UserData&, UserData&)> PhysicsEngine::myEndContactCallback;

    std::function<void(UserData&, UserData&)> PhysicsEngine::myBeginTriggerCallback;
    std::function<void(UserData&, UserData&)> PhysicsEngine::myEndTriggerCallback;

    std::array<uint64_t, MAX_LAYERS> PhysicsEngine::myCollisionLayers = {};

    b2WorldId PhysicsEngine::myWorld;
    Math::Vector2f PhysicsEngine::myGravity;
    int PhysicsEngine::mySubstepCount = 8;

    b2DebugDraw PhysicsEngine::myDebugDraw;
    bool PhysicsEngine::myDrawDebugShapes = true;
    bool PhysicsEngine::myDrawQueries = false;

    bool PhysicsEngine::myHasCreatedWorld;

    void Eclipse::PhysicsEngine::ProcessContactEvents()
    {

    }

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

        int result = memcmp(layers.data(), aCollisionLayers.data(), MAX_LAYERS * sizeof(uint64_t));

        if (!result)
            memset(aCollisionLayers.data(), ULLONG_MAX, MAX_LAYERS * sizeof(uint64_t));
        else
            memcpy(aCollisionLayers.data(), layers.data(), MAX_LAYERS * sizeof(uint64_t));
    }

    void PhysicsEngine::InitWorld()
    {

        b2WorldDef worldDef;
        worldDef = b2DefaultWorldDef();
        worldDef.gravity = b2Vec2(myGravity.x, myGravity.y);

        worldDef.contactHertz = 240.f;

        worldDef.enableContinuous = true;

        b2SetLengthUnitsPerMeter(0.01f);

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

        myDebugDraw.drawShapes = true;

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
        const float deltaTime = Timer::GetDeltaTime();

        b2World_Step(myWorld, deltaTime, mySubstepCount);

        CheckCollisions();
    }
}