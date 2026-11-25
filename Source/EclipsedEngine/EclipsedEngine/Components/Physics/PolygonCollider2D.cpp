#include "PolygonCollider2D.h"

#include "Physics/PhysicsEngine.h"

#include "EclipsedEngine/Components/Transform2D.h"

namespace Eclipse
{
    void PolygonCollider2D::CreateCollider()
    {
        OnTransformDirty();
    }

    void PolygonCollider2D::AddPoint(const Math::Vector2f& aPoint)
    {
        Transform2D* transform = gameObject->GetComponent<Transform2D>();
        Math::Vector2f scale = (Math::Vector2f(transform->GetScale().x, transform->GetScale().y) * 0.1f);

        scale *= aPoint * Math::Vector2f(2.f, 2.f) - Math::Vector2f(1.f, 1.f);

        myPoints.emplace_back(scale);
    }

    void PolygonCollider2D::DeltaChanges()
    {
        if (myLastPoints.empty() || memcmp(myPoints.data(), myLastPoints.data(), myPoints.size() * sizeof(Math::Vector2f)))
        {
            myLastPoints.resize(myPoints.size());
            memcpy(myLastPoints.data(), myPoints.data(), myPoints.size() * sizeof(Math::Vector2f));

            OnShapeDirty();
        }
    }

    void PolygonCollider2D::OnShapeDirty()
    {
        Math::Vector2f scale = Math::Vector2f(myTransform->GetScale().x, myTransform->GetScale().y) * 0.01f;

        if (!CreatedShape)
            CreatedShape = PhysicsEngine::CreatePolygonCollider(myInternalCollider, myBodyRef, myPoints, myLayer);

        PhysicsEngine::SetTransformPolygon(myInternalCollider, myPoints, scale);
    }
}