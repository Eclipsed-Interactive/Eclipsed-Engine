#include "CapsuleCollider2D.h"

#include "Physics/PhysicsEngine.h"

#include "EclipsedEngine/Components/Transform2D.h"

#undef max

namespace Eclipse
{
	void CapsuleCollider2D::CreateCollider()
	{
		PhysicsEngine::CreateCapsuleCollider(myInternalCollider, myBodyRef, isTrigger, Radius, HalfHeight, myLayer);

		OnTransformDirty();
	}

	void CapsuleCollider2D::EditorUpdate()
	{
		if (!myTransform)
			return;

		if (myLastRadius != Radius)
		{
			OnShapeDirty();
		}

		if (myLastHalfHeight != HalfHeight)
		{
			OnShapeDirty();
		}

		if (myLastColliderPivot.x != ColliderPivot.x || myLastColliderPivot.y != ColliderPivot.y)
		{
			OnShapeDirty();
		}
	}

	void CapsuleCollider2D::SetRadius(float aRadius)
	{
		Transform2D* transform = gameObject->GetComponent<Transform2D>();
		Math::Vector2f size = Math::Vector2f(transform->GetScale().x, transform->GetScale().y) * 0.01f;

		Radius = aRadius;
	}

	void CapsuleCollider2D::SetHalfHeight(float aHalfHeight)
	{
		Transform2D* transform = gameObject->GetComponent<Transform2D>();
		Math::Vector2f size = Math::Vector2f(transform->GetScale().x, transform->GetScale().y) * 0.01f;

		HalfHeight = aHalfHeight;
	}

	void CapsuleCollider2D::OnShapeDirty()
	{
		myLastRadius = Radius;
		myLastHalfHeight = HalfHeight;
		myLastColliderPivot = ColliderPivot;

		Math::Vector2f size = Math::Vector2f(myTransform->GetScale().x, myTransform->GetScale().y) * 0.01f;

		float radius = Radius * std::max(size.x, size.y);
		float halfHeight = HalfHeight * size.y;

		PhysicsEngine::SetTransformCapsule(myInternalCollider, radius, halfHeight);
	}
}