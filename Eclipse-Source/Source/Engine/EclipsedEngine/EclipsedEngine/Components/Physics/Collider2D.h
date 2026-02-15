#pragma once

#include "EclipsedEngine/Components/Component.h"

#include "PhysicsEngine/PhysicsEngineSettings.h"

struct b2BodyId;
struct b2ShapeId;

namespace Eclipse
{
    class ECLIPSED_API Collider2D : public Component
    {
        BASE_SELECTION(Collider2D, 5)

    public:
        void OnDestroy();

        void OnComponentAdded() override;

        void EditorUpdate() override;

        virtual void OnTransformDirty() {}

        bool IsBodyOwner() { return BodyOwned; }
        b2BodyId* GetBody() { return myBodyRef; }

    protected:
        virtual void CreateCollider() {}
        virtual void DeltaChanges() {}

    public:
        SERIALIZED_FIELD_DEFAULT(Layer, myLayer, Layer::Default);
        SERIALIZED_FIELD(Math::Vector2f, ColliderPivot);

    protected:

        UserData myUserData;
        Math::Vector2f myLastColliderPivot;
        
        class Transform2D* myTransform;

        ShapeId myShapeID;
        BodyId myBodyID;
        
        // Internal
        b2ShapeId* myInternalCollider = reinterpret_cast<b2ShapeId*>(&myShapeID);
        b2BodyId* myBodyRef = reinterpret_cast<b2BodyId*>(&myBodyID);

    private:
        int myLastLayer;

        bool BodyOwned = false;
    };
}
