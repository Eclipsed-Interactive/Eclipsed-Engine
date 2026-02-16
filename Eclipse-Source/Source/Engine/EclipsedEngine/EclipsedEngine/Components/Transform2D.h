#pragma once

#include "EclipsedEngine/Components/Component.h"

#include <vector>
#include <functional>

#include "CoreEngine/Math/Vector/Vector2.h"
#include "CoreEngine/Math/Vector/Vector3.h"
#include "CoreEngine/Math/Matrix/Matrix3x3.h"

namespace Eclipse
{
	using DirtyFunctionPtr = void (Component::*)();

	class Transform2D : public Component
	{
		COMPONENT_BASE_2(Transform2D, UINT_MAX * 0.5f)

	public:
		void OnComponentAddedNoCreations();

		void AfterRenderUpdate() override;
		void DirtyUpdate() const;


		void AddParentTransform(GameObject* aParent, Math::Mat3x3f& aTransform) const;
		void AddParentRotation(GameObject* aParent, float& totalRotation) const;
		void AddParentScale(GameObject* aParent, Math::Vector2f& totalScale) const;

		Math::Matrix3x3f GetTransformMatrix() const;

		//Global
		Math::Vector2f GetPosition() const;
		float GetRotation() const;
		Math::Vector2f GetScale() const;

		//Local
		const Math::Vector2f& GetLocalPosition() const;
		const float GetLocalRotation() const;
		const Math::Vector2f& GetLocalScale() const;

		Math::Vector2f* GetPositionPtr();
		float* GetRotationPtr();
		Math::Vector2f* GetScalePtr();

		void SetPosition(const Math::Vector2f& aPosition);
		void SetPosition(float aX, float aY);

		void SetRotation(float aRotation);

		void SetScale(const Math::Vector2i& aScale);
		void SetScale(const Math::Vector2f& aScale);
		void SetScale(float aX, float aY);

		void AddFunctionToRunOnDirtyUpdate(const std::function<void()>& aFunction);
		void AddFunctionToRunOnDirtyUpdate(DirtyFunctionPtr* aFunction);

	private:
		REPLICATED_SERIALIZED_FIELD_STEP_DEFAULT(Math::Vector2<float>, position, 0.01f, Math::Vector2f(0, 0), Transform2D);
		REPLICATED_SERIALIZED_FIELD_STEP_DEFAULT(float, rotation, 0.01f, 0, Transform2D);
		REPLICATED_SERIALIZED_FIELD_DEFAULT(Math::Vector2f, scale, Math::Vector2f(1, 1), Transform2D);

		Math::Vector2f lastPosition;
		float lastRotation;
		Math::Vector2f lastScale;

	private:
		bool myIsDirty = true;
		std::vector<std::function<void()>> myFunctionsToRunOnDirtyUpdate;
	};
}