#include "RigidBody2D.h"

#include "PhysicsEngine/PhysicsEngine.h"
#include "EclipsedEngine/Components/Transform2D.h"

#include "Collider2D.h"

namespace Eclipse
{
    void RigidBody2D::OnDestroy()
    {
        Physics::SetLinearVelocity(myBody, myVelocity);
        PhysicsEngine::ChangeBodyType(myBody, BodyType::Static);
    }

    void RigidBody2D::Awake()
    {
        std::vector<Collider2D*> colliders;
        ComponentManager::GetAllComponentsOfType<Collider2D>(gameObject->GetID(), colliders);
        for (auto& collider : colliders)
        {
            if (collider->IsBodyOwner())
            {
                myBody = collider->GetBody();
                break;
            }
        }
        
        PhysicsEngine::ChangeRBLocks(myBody, LockXPos, LockYPos, LockRotation);
        
        PhysicsEngine::ChangeBodyType(myBody, BodyType::Dynamic);

        myTransform = gameObject->GetComponent<Transform2D>();

        myTransform->AddFunctionToRunOnDirtyUpdate(this, [&]() {
            PhysicsEngine::SetTransform(myBody, myTransform->GetPosition(), myTransform->GetRotation());
            });
    }

    void RigidBody2D::EarlyUpdate()
    {
        myVelocity = Physics::GetLinearVelocity(myBody);
        myAngularVelocity = Physics::GetAngularVelocity(myBody);

        Math::Vector2f position = Physics::GetBodyPosition(myBody);
        myTransform->SetPosition(position);
        float rotation = Physics::GetBodyRotation(myBody);
        myTransform->SetRotation(rotation);
    }

    void RigidBody2D::SetVelocity(const Math::Vector2f& aVelocity)
    {
        myVelocity = aVelocity;
        Physics::SetLinearVelocity(myBody, myVelocity);
    }

    void RigidBody2D::AddForce(const Math::Vector2f& aVelocity)
    {
        SetVelocity(myVelocity + aVelocity);
    }

    const Math::Vector2f& RigidBody2D::GetVelocity()
    {
        return myVelocity;
    }

    void RigidBody2D::SetAngularVelocity(float aAngularVelocity)
    {
        myAngularVelocity = aAngularVelocity;
        Physics::SetAngularVelocity(myBody, aAngularVelocity);
    }

    const float RigidBody2D::GetAngularVelocity()
    {
        return myAngularVelocity;
    }

    void RigidBody2D::SetRotationLocked(bool aValue)
    {
        myRigidBodySettings.LockRotation = aValue;
        PhysicsEngine::ChangeRBLocks(myBody, myRigidBodySettings.LockXPos, myRigidBodySettings.LockYPos, myRigidBodySettings.LockRotation);
    }
    bool RigidBody2D::GetRotationLocked()
    {
        return myRigidBodySettings.LockRotation;
    }

    void RigidBody2D::SetXPosLocked(bool aValue)
    {
        myRigidBodySettings.LockXPos = aValue;
        PhysicsEngine::ChangeRBLocks(myBody, myRigidBodySettings.LockXPos, myRigidBodySettings.LockYPos, myRigidBodySettings.LockRotation);
    }
    bool RigidBody2D::GetXPosLocked()
    {
        return myRigidBodySettings.LockXPos;
    }

    void RigidBody2D::SetYPosLocked(bool aValue)
    {
        myRigidBodySettings.LockYPos = aValue;
        PhysicsEngine::ChangeRBLocks(myBody, myRigidBodySettings.LockXPos, myRigidBodySettings.LockYPos, myRigidBodySettings.LockRotation);
    }
    bool RigidBody2D::GetYPosLocked()
    {
        return myRigidBodySettings.LockYPos;
    }
}