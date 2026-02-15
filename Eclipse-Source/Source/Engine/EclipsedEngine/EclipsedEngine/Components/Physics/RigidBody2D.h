#pragma once

#include "EclipsedEngine/Components/Component.h"
#include "CoreEngine/Math/Vector/Vector2.h"

#include "PhysicsEngine/PhysicsEngineSettings.h"

struct b2BodyId;

namespace Eclipse
{
    class ECLIPSED_API RigidBody2D : public Component
    {
        BASE_SELECTION(RigidBody2D, 5)

    public:
        void OnDestroy() override;
        void Awake() override;

        void EarlyUpdate() override;

        void AddForce(const Math::Vector2f& aVelocity);

        void SetVelocity(const Math::Vector2f& aVelocity);
        const Math::Vector2f& GetVelocity();

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

        SERIALIZED_FIELD_DEFAULT(bool, LockRotation, false);
        SERIALIZED_FIELD_DEFAULT(bool, LockXPos, false);
        SERIALIZED_FIELD_DEFAULT(bool, LockYPos, false);
        SERIALIZED_FIELD(Math::Vector2f, myVelocity);
        
        //Math::Vector2f myVelocity = { 0.f, 0.f };

        float myAngularVelocity = 0.f;

    private:
        class Transform2D* myTransform;

    private:
        // Internals
        UserData myUserData;
        bool bodyHasBeenCreated = false;
    };
}