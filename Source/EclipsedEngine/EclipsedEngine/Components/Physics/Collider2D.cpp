#include "Collider2D.h"

#include "RigidBody2D.h"

#include "ECS/ComponentManager.h"

#include "Physics/PhysicsEngine.h"
#include "EclipsedEngine/Components/Transform2D.h"
#include "Physics/PhysicsEngineSettings.h"

namespace Eclipse
{
    void Collider2D::OnDestroy()
    {
        if (BodyOwned)
        {
            PhysicsEngine::DeleteShape(myInternalCollider);

            std::vector<Collider2D*> colliders;
            ComponentManager::GetAllComponentsOfType<Collider2D>(gameObject->GetID(), colliders);

            if (!colliders.size())
                PhysicsEngine::DeleteBody(myBodyRef);
        }
        else
        {
            PhysicsEngine::DeleteShape(myInternalCollider);
        }
    }

    void Collider2D::OnComponentAdded()
    {
        myTransform = gameObject->transform;
        myLastLayer = static_cast<int>(myLayer);

        myUserData = { gameObject->GetID() };

        std::vector<Collider2D*> colliders;
        ComponentManager::GetAllComponentsOfType<Collider2D>(gameObject->GetID(), colliders);

        bool ColliderHasRB = false;

        for (auto& collider : colliders)
        {
            if (collider->BodyOwned)
            {
                myBodyRef = collider->myBodyRef;
                ColliderHasRB = true;
                break;
            }
        }

        if (!ColliderHasRB)
        {
            PhysicsEngine::CreateRigidBody(myBodyRef, &myUserData, StaticBody, false, false, false, myTransform->GetPosition());
            BodyOwned = true;
        }

        myTransform->AddFunctionToRunOnDirtyUpdate(this, [this]() {
            this->OnTransformDirty();
            });

        CreateCollider();

        PhysicsEngine::SetPhysicsMaterial(myInternalCollider, physMaterial);
    }

    void Collider2D::EditorUpdate()
    {
        DeltaChanges();

        if (myLastLayer != static_cast<int>(myLayer))
        {
            myLastLayer = static_cast<int>(myLayer);
            PhysicsEngine::ChangeLayer(myInternalCollider, myLayer);
        }
    }

    void Collider2D::OnTransformDirty()
    {
        if (BodyOwnedByRB)
            return;

        PhysicsEngine::SetTransform(myBodyRef, myTransform->GetPosition(), myTransform->GetRotation());
        OnShapeDirty();
    }
}