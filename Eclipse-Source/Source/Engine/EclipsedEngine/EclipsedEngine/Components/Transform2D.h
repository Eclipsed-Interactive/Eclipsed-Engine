#pragma once

#include "EclipsedEngine/Components/Component.h"

#include <vector>
#include <functional>

#include "CoreEngine/Math/Vector/Vector2.h"
#include "CoreEngine/Math/Vector/Vector3.h"
#include "CoreEngine/Math/Matrix/Matrix3x3.h"
#include "CoreEngine/Macros/defines.h"

namespace Eclipse
{
	using DirtyFunctionPtr = void (Component::*)();

	class ECLIPSED_API Transform2D : public Component
	{
		COMPONENT_BASE_2(Transform2D, UINT_MAX * 0.5f)

	public:
		void OnComponentAddedNoCreations();

		void EditorUpdate() override;
		void DirtyUpdate();

		//Global
		Math::Vector2f GetPosition();
		float GetRotation();
		Math::Vector2f GetScale();
		Math::Matrix3x3f GetTransformMatrix();

		//Local
		const Math::Vector2f& GetLocalPosition() const;
		const float GetLocalRotation() const;
		const Math::Vector2f& GetLocalScale() const;
		Math::Matrix3x3f GetLocalTransformMatrix() const;

		Math::Vector2f* GetPositionPtr();
		float* GetRotationPtr();
		Math::Vector2f* GetScalePtr();

		void SetPosition(const Math::Vector2f& aPosition);
		void SetPosition(float aX, float aY);

		void SetRotation(float aRotation);

		void SetScale(const Math::Vector2i& aScale);
		void SetScale(const Math::Vector2f& aScale);
		void SetScale(float aX, float aY);

		void AddFunctionToRunOnDirtyUpdate(Component* aComponent, const std::function<void()>& aFunction);
		//void AddFunctionToRunOnDirtyUpdate(DirtyFunctionPtr* aFunction);

		static void ChildingObject(GameObject* aChild, GameObject* aParent);
		
	private:
		void AddParentTransform(GameObject* aParent, Math::Mat3x3f& aTransform) const;
		void AddParentRotation(GameObject* aParent, float& totalRotation) const;
		void AddParentScale(GameObject* aParent, Math::Vector2f& totalScale) const;

		void UpdateTransforms();

		Math::Matrix3x3f GetTransform() const;
		
	private:
		REPLICATED_SERIALIZED_FIELD_STEP_DEFAULT(Math::Vector2<float>, position, 0.01f, Math::Vector2f(0, 0), Transform2D);
		REPLICATED_SERIALIZED_FIELD_STEP_DEFAULT(float, rotation, 5.f, 0, Transform2D);
		REPLICATED_SERIALIZED_FIELD_DEFAULT(Math::Vector2f, scale, Math::Vector2f(1, 1), Transform2D);

		Math::Vector2f lastPosition;
		float lastRotation;
		Math::Vector2f lastScale;

		Math::Vector2f GlobalPosition;
		float GlobalRotation;
		Math::Vector2f GlobalScale;
		
		Math::Matrix3x3f GlobalTransformationMatrix;
		
		// Not implemented but if strictly necessary easy to do
		//Math::Matrix3x3f LocalTransformationMatrix;
		
	private:
		struct DirtyTransformContainer
		{
			Component* component;
			std::function<void()> function;
		};
		
		bool myIsDirty = true;
		std::vector<DirtyTransformContainer> myFunctionsToRunOnDirtyUpdate;
	};
}