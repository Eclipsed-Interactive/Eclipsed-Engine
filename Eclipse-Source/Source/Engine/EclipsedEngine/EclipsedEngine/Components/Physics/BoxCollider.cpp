#include "BoxCollider2D.h"

#include "PhysicsEngine/PhysicsEngine.h"
#include "EclipsedEngine/Components/Transform2D.h"

#include "RigidBody2D.h"

namespace Eclipse
{
	void BoxCollider2D::DeltaChanges()
	{
		if (myLastHalfExtents.x != HalfExtents->x || myLastHalfExtents.y != HalfExtents->y)
		{
			OnTransformDirty();
		}

		if (myLastColliderPivot.x != ColliderPivot->x || myLastColliderPivot.y != ColliderPivot->y)
		{
			OnTransformDirty();
		}
	}

	void BoxCollider2D::CreateCollider()
	{
		SetScale(HalfExtents);
		PhysicsEngine::CreateBoxCollider(myInternalCollider, myBodyRef, myHalfExtents, myLayer);
	}

	void BoxCollider2D::SetScale(const Math::Vector2f& aHalfExtents)
	{
		myLastHalfExtents = HalfExtents;
		HalfExtents = aHalfExtents;

		Math::Vector2f halfExtent = Math::Vector2f(myTransform->GetScale().x, myTransform->GetScale().y) * 0.01f;

		halfExtent.x *= aHalfExtents.x;
		halfExtent.y *= aHalfExtents.y;

		myHalfExtents = halfExtent;
	}

	void BoxCollider2D::OnTransformDirty()
	{
		myLastHalfExtents = HalfExtents;
		myLastColliderPivot = ColliderPivot;

		Math::Vector2f halfExtent = Math::Vector2f(myTransform->GetScale().x, myTransform->GetScale().y) * 0.01f;

		halfExtent.x *= HalfExtents->x;
		halfExtent.y *= HalfExtents->y;

		myHalfExtents = halfExtent;

		PhysicsEngine::SetTransformBox(myBodyRef, myTransform->GetPosition(), myTransform->GetRotation(), myHalfExtents, { ColliderPivot->x * halfExtent.x * 2.f, ColliderPivot->y * halfExtent.y * 2.f });
	}
}