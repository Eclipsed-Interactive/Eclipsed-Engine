#pragma once

#include "EclipsedEngine/Components/Component.h"
#include "Core/Math/Vector/Vector2.h"

#include "Physics/PhysicsEngineSettings.h"
#include "EclipsedEngine/Reflection/Reflection_Macros.h"
#include "EclipsedEngine/Replication/Replication_Macros.h"
struct b2BodyId;

namespace Eclipse
{
    class ECL_API RigidBody2D : public Component
    {
        MAKE_REFLECTABLE(RigidBody2D);
        REFLECTION_FLAG(COMPONENT);

        BASE_SELECTION(RigidBody2D, 5)

    public:
        void OnDestroy() override;
        void Awake() override;

        void EarlyUpdate() override;

        void AddForce(const Math::Vector2f& aVelocity);

        void SetVelocity(const Math::Vector2f& aVelocity);
        const Math::Vector2f& GetVelocity();

        void SetBodyPosition(const Math::Vector2f& aPosition);

        void SetAngularVelocity(float aAngularVelocity);
        const float GetAngularVelocity();

        void SetBodyType(Box2DBodyType aBodytype);
        void GetBodyType();

        void SetRotationLocked(bool aValue);
        bool GetRotationLocked();

        void SetXPosLocked(bool aValue);
        bool GetXPosLocked();

        void SetYPosLocked(bool aValue);
        bool GetYPosLocked();

        bool ColliderAttached = false;

    private:
        b2BodyId* myBody;

        RigidBodySettings myRigidBodySettings;

        Box2DBodyType BodyType = DynamicBody;

        [[SerializeField]] bool LockRotation = false;
        [[SerializeField]] bool LockXPos = false;
        [[SerializeField]] bool LockYPos = false;
        
        Math::Vector2f myVelocity = { 0.f, 0.f };

        float myAngularVelocity = 0.f;

    private:
        class Transform2D* myTransform;

    private:
        // Internals
        UserData myUserData;
        bool bodyHasBeenCreated = false;
    };
}