#pragma once

#include "EclipsedEngine/Components/Component.h"
#include "Physics/PhysicsEngineSettings.h"

#include "EclipsedEngine.Core.hpp"

struct b2BodyId;
struct b2ShapeId;

namespace Eclipse
{
    class ECL_API Collider2D : public Component
    {
        BASE_SELECTION(Collider2D, 5)

    public:
        void OnDestroy();

        void OnComponentAdded() override;

        void EditorUpdate() override;

        void OnTransformDirty();
        virtual void OnShapeDirty() {}

        bool IsBodyOwner() { return BodyOwned; }
        void SetRBOwningCollider(bool aOwned) { BodyOwnedByRB = aOwned; }
        b2BodyId* GetBody() { return myBodyRef; }

    protected:
        virtual void CreateCollider() {}
        virtual void DeltaChanges() {}

    public:
        [[SerializeField]]
        Layer myLayer = Layer::Default;

        [[SerializeField]]
        Math::Vector2f ColliderPivot;

        PhysMaterial physMaterial;

    protected:

        UserData myUserData;
        Math::Vector2f myLastColliderPivot;
        
        class Transform2D* myTransform;

        ShapeId myShapeID;
        BodyId myBodyID;
        
        // Internal
        b2ShapeId* myInternalCollider = reinterpret_cast<b2ShapeId*>(&myShapeID);
        b2BodyId* myBodyRef = reinterpret_cast<b2BodyId*>(&myBodyID);

        bool BodyOwnedByRB = false;

    private:
        int myLastLayer;
        bool BodyOwned = false;
    };
}
