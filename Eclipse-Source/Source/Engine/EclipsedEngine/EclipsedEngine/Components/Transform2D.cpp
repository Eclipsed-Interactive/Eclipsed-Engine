#include "Transform2D.h"

#include <sstream>

#include "CoreEngine/Math/Vector/Vector3.h"

namespace Eclipse
{
	void Transform2D::position_OnRep()
	{

	}
	void Transform2D::rotation_OnRep()
	{

	}
	void Transform2D::scale_OnRep()
	{

	}

	void Transform2D::OnComponentAddedNoCreations()
	{
		gameObject->transform = this;
	}

	void Transform2D::AddParentTransform(GameObject* aParent, Math::Mat3x3f& aTransform) const
	{
		Transform2D* parentTransform = aParent->GetComponent<Transform2D>();
		if (!parentTransform)
			return;

		Math::Mat3x3f parentTransformMatrix = Math::Mat3x3f::CreateTranslation(parentTransform->GetLocalPosition());
		parentTransformMatrix *= Math::Mat3x3f::CreateRotation(-parentTransform->GetLocalRotation());
		//parentTransformMatrix *= Math::Mat3x3f::CreateScale(parentTransform->GetLocalScale());

		aTransform *= parentTransformMatrix;

		GameObject* parent = aParent->GetParent();
		if (parent && parent->transform)
			AddParentTransform(parent, aTransform);
	}

	Math::Vector2f Transform2D::GetPosition() const
	{
		Math::Vector3f positionV3(position->x, position->y, 1.f);

		GameObject* parent = gameObject->GetParent();
		if (parent && parent->transform)
			positionV3 = positionV3 * parent->transform->GetTransformMatrix();

		return { positionV3.x, positionV3.y };
	}

	Math::Matrix3x3f Transform2D::GetTransformMatrix() const
	{
		Math::Mat3x3f mat = Math::Mat3x3f::CreateTranslation(GetLocalPosition());
		mat *= Math::Mat3x3f::CreateRotation(-GetLocalRotation());

		GameObject* parent = gameObject->GetParent();
		if (parent && parent->transform)
			AddParentTransform(parent, mat);

		return mat;
	}

	void Transform2D::AddParentRotation(GameObject* aParent, float& totalRotation) const
	{
		Transform2D* parentTransform = aParent->GetComponent<Transform2D>();
		if (!parentTransform)
			return;

		totalRotation += parentTransform->GetLocalRotation();

		GameObject* parent = aParent->GetParent();
		if (parent && parent->transform)
			AddParentRotation(parent, totalRotation);
	}

	float Transform2D::GetRotation() const
	{
		float rot = rotation;
		GameObject* parent = gameObject->GetParent();
		if (parent && parent->transform)
			AddParentRotation(parent, rot);

		return rot;
	}

	void Transform2D::AddParentScale(GameObject* aParent, Math::Vector2f& totalScale) const
	{
		Transform2D* parentTransform = aParent->GetComponent<Transform2D>();
		if (!parentTransform)
			return;

		totalScale *= parentTransform->GetLocalScale();

		GameObject* parent = aParent->GetParent();
		if (parent && parent->transform)
			AddParentScale(parent, totalScale);
	}

	Math::Vector2f Transform2D::GetScale() const
	{
		Math::Vector2f sca = scale;
		GameObject* parent = gameObject->GetParent();
		if (parent && parent->transform)
			AddParentScale(parent, sca);

		return sca;
	}

	const Math::Vector2f& Transform2D::GetLocalPosition() const
	{
		return position;
	}
	const float Transform2D::GetLocalRotation() const
	{
		return rotation;
	}
	const Math::Vector2f& Transform2D::GetLocalScale() const
	{
		return scale;
	}

	Math::Vector2f* Transform2D::GetPositionPtr()
	{
		return &position.Get();
	}
	float* Transform2D::GetRotationPtr()
	{
		return &rotation.Get();
	}
	Math::Vector2f* Transform2D::GetScalePtr()
	{
		return &scale.Get();
	}

	void Transform2D::SetPosition(const Math::Vector2f& aPosition)
	{
		SetPosition(aPosition.x, aPosition.y);
	}
	void Transform2D::SetPosition(float aX, float aY)
	{
		position->x = aX;
		position->y = aY;

		lastPosition = position;

		myIsDirty = true;
	}

	void Transform2D::SetRotation(float aRotation)
	{
		rotation = aRotation;

		lastRotation = rotation;

		myIsDirty = true;
	}

	void Transform2D::SetScale(const Math::Vector2i& aScale)
	{
		SetScale(static_cast<float>(aScale.x), static_cast<float>(aScale.y));
	}
	void Transform2D::SetScale(const Math::Vector2f& aScale)
	{
		SetScale(aScale.x, aScale.y);
	}
	void Transform2D::SetScale(float aX, float aY)
	{
		scale->x = aX;
		scale->y = aY;

		lastScale = scale;

		myIsDirty = true;
	}

	void Transform2D::AddFunctionToRunOnDirtyUpdate(const std::function<void()>& aFunction)
	{
		myFunctionsToRunOnDirtyUpdate.push_back(aFunction);
	}

	void Transform2D::AfterRenderUpdate()
	{
		if (position->x != lastPosition.x || position->y != lastPosition.y)
		{
			lastPosition = position;
			myIsDirty = true;


		}
		if (scale->x != lastScale.x || scale->y != lastScale.y)
		{
			lastScale = scale;
			myIsDirty = true;


		}
		if (lastRotation != rotation)
		{
			lastRotation = rotation;
			myIsDirty = true;
		}


		if (myIsDirty)
		{
			DirtyUpdate();
			myIsDirty = false;
		}
	}

	void Transform2D::DirtyUpdate() const
	{
		for (auto& func : myFunctionsToRunOnDirtyUpdate)
			func();

		for (const auto& child : gameObject->GetChildren())
		{
			if(child->transform)
				child->transform->DirtyUpdate();
		}
	}
}