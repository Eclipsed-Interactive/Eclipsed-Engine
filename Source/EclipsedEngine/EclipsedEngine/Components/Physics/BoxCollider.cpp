#include "BoxCollider2D.h"

#include "EclipsedEngine/Components/Transform2D.h"

#include "Physics/PhysicsEngine.h"

namespace Eclipse
{
	void BoxCollider2D::DeltaChanges()
	{
		if (myLastHalfExtents.x != HalfExtents.x || myLastHalfExtents.y != HalfExtents.y)
		{
			OnShapeDirty();
		}

		if (myLastColliderPivot.x != ColliderPivot.x || myLastColliderPivot.y != ColliderPivot.y)
		{
			OnShapeDirty();
		}
	}

	void BoxCollider2D::CreateCollider()
	{
		SetScale(HalfExtents);
		PhysicsEngine::CreateBoxCollider(myInternalCollider, myBodyRef, isTrigger, myHalfExtents, myLayer);

		OnShapeDirty();
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

	void BoxCollider2D::OnShapeDirty()
	{
		myLastHalfExtents = HalfExtents;
		myLastColliderPivot = ColliderPivot;

		Math::Vector2f halfExtent = Math::Vector2f(myTransform->GetScale().x, myTransform->GetScale().y) * 0.01f;

		halfExtent.x *= HalfExtents.x;
		halfExtent.y *= HalfExtents.y;

		myHalfExtents = halfExtent;

		PhysicsEngine::SetTransformBox(myInternalCollider, myHalfExtents, { ColliderPivot.x * halfExtent.x * 2.f, ColliderPivot.y * halfExtent.y * 2.f });
	}
}