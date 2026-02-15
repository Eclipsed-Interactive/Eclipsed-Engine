#include "Collider2D.h"

#include "RigidBody2D.h"

#include "EntityEngine/ComponentManager.h"

#include "PhysicsEngine/PhysicsEngine.h"
#include "EclipsedEngine/Components/Transform2D.h"

namespace Eclipse
{
    void Collider2D::OnDestroy()
    {
        auto rigidBody = gameObject->GetComponent<RigidBody2D>();
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
        myLastLayer = static_cast<int>(myLayer->value);

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

        myTransform->AddFunctionToRunOnDirtyUpdate([this]() {
            this->OnTransformDirty();
            });

        CreateCollider();
    }

    void Collider2D::EditorUpdate()
    {
        DeltaChanges();

        if (myLastLayer != static_cast<int>(myLayer->value))
        {
            myLastLayer = myLayer->value;
            PhysicsEngine::ChangeLayer(myInternalCollider, myLayer);
        }
    }
}