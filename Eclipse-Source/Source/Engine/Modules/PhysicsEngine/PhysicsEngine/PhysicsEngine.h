#pragma once

#include "PhysicsEngineSettings.h"
#include "CoreEngine/Math/Vector/Vector2.h"

#include "box2d/id.h"
#include "box2d/types.h"

#include <vector>
#include <functional>

#include <array>



namespace Eclipse
{
    class PhysicsEngine
    {
    public:
        class Physics
        {
        public:
            static void SetLinearVelocity(const b2BodyId* aBodyID, const Math::Vector2f& aVelocity);
            static void SetAngularVelocity(const b2BodyId* aBodyID, float aVelocity);

            static Math::Vector2f GetLinearVelocity(const b2BodyId* aBodyID);
            static float GetAngularVelocity(const b2BodyId* aBodyID);

            static Math::Vector2f GetBodyPosition(const b2BodyId* aBodyID);
            static float GetBodyRotation(const b2BodyId* aBodyID);
        };

    public:
        static bool RayCast(const Ray& aRay, HitResults& aHitResults, float length, Layer aLayerMask = Layer::All);
        static bool OverlapBox(const Math::Vector2f& aPositon, const Math::Vector2f& aHalfExent, HitResults& aHitResults, Layer aLayerMask = Layer::All);
        static bool OverlapSphere(const Math::Vector2f& aPositon, float aRadius, HitResults& aHitResults, Layer aLayerMask = Layer::All);



        static void CreateRigidBody(b2BodyId* aBody,
            UserData* aUserData,
            Box2DBodyType BodyType = StaticBody,
            bool LockRotation = false,
            bool LockXPos = false,
            bool LockYPos = false,
            const Math::Vector2f& aStartPosition = { 0.f, 0.f });

        // Simple
        static void CreateBoxCollider(b2ShapeId* aShape, const b2BodyId* aBodyID, const Math::Vector2f& aHalfExtents, Layer aLayer);
        static void CreateCircleCollider(b2ShapeId* aShape, const b2BodyId* aBodyID, float radius, Layer aLayer);
        static void CreateCapsuleCollider(b2ShapeId* aShape, const b2BodyId* aBodyID, float aHalfHeight, float aRadius, Layer aLayer);

        // Complex
        static bool CreatePolygonCollider(b2ShapeId* aShape, const b2BodyId* aBodyID, const std::vector<Math::Vector2f>& aPolygonPoints, Layer aLayer);

        static void DeleteShape(b2ShapeId* aShape);
        static void DeleteBody(b2BodyId* aBody);
        
        static void ChangeLayer(b2ShapeId* aShapeID, Layer aLayer);
        static void ChangeBodyType(b2BodyId* aBodyID, BodyType aBodyType);
        static void ChangeRBLocks(b2BodyId* aBodyID, bool XLock, bool YLock, bool RotationLock);

        static void RemoveRigidBody(b2BodyId* aBodyID);
        static void RemoveCollider(b2ShapeId* aShape);

        static void SetPosition(b2BodyId* aBodyID, const Math::Vector2f& aPosition);
        static void SetRotation(b2BodyId* aBodyID, float aRotation);

        static void SetTransform(b2BodyId* aBodyID, const Math::Vector2f& aPosition, float aRotation);

        // Update Simple collisions
        static void SetTransformBox(b2BodyId* aBodyID, const Math::Vector2f& aPosition, float aRotation, const Math::Vector2f& aScale, const Math::Vector2f& aPivot = { 0.f, 0.f });
        static void SetTransformCircle(b2BodyId* aBodyID, const Math::Vector2f& aPosition, float aRotation, float aRadius, const Math::Vector2f& aPivot = { 0.f, 0.f });
        static void SetTransformCapsule(b2BodyId* aBodyID, const Math::Vector2f& aPosition, float aRotation, float aRadius, float aHalfHeight, const Math::Vector2f& aPivot = { 0.f, 0.f });

        // Update Complex collisions
        static void SetTransformPolygon(b2BodyId* aBodyID, const Math::Vector2f& aPosition, float aRotation, const std::vector<Math::Vector2f>& aPoints, const Math::Vector2f& aScale, const Math::Vector2f& aPivot = { 0.f, 0.f });

        static void SetGravity(const Math::Vector2f& aGravity);

        static void InitWorld();
        static void Init(int aSubstepCount, const Math::Vector2f& aGravity, b2DebugDraw& aDebugdraw);
        static void Update();
        static void CleanUp();
        static void DrawPhysicsObjects();


        static void CheckCollisions();

        static bool& GetDebugDraw();
        static bool& GetDebugDrawShapes(DebugDrawTypes aType);

        static void LoadLayers();

        static inline std::function<void(UserData&)> myBeginContactCallback;
        static inline std::function<void(UserData&)> myEndContactCallback;

        static inline std::array<uint64_t, MAX_LAYERS> myCollisionLayers = {};

    private:
        static inline b2WorldId myWorld;
        static inline Math::Vector2f myGravity;
        static inline int mySubstepCount = 8;

        static inline b2DebugDraw myDebugDraw;
        static inline bool myDrawDebugShapes = true;
        static inline bool myDrawQueries = false;

        static inline bool myHasCreatedWorld;
    };
}