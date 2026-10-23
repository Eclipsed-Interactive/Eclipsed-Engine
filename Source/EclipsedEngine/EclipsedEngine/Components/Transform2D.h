#pragma once

#include "EclipsedEngine/Components/Component.h"

#include <vector>
#include <functional>

#include "Core/Math/Vector/Vector2.h"
#include "Core/Math/Vector/Vector3.h"
#include "Core/Math/Matrix/Matrix3x3.h"

#include "EclipsedEngine.Core.hpp"
#include "Core/GraphicsBuffers/TransformBuffer.h"

#include "EclipsedEngine/Reflection/Reflection_Macros.h"
#include "EclipsedEngine/Replication/Replication_Macros.h"

namespace Eclipse
{
	using DirtyFunctionPtr = void (Component::*)();

	class ECL_API Transform2D : public Component
	{
		MAKE_REFLECTABLE(Transform2D);
		REFLECTION_FLAG(COMPONENT);

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
		[[SerializeField, Step(0.01f)]]
		Math::Vector2f position = Math::Vector2f(0.f, 0.f);

		[[SerializeField, Step(5.f)]]
		float rotation = 0.f;

		[[SerializeField]]
		Math::Vector2f scale = Math::Vector2f(1.f, 1.f);

		Math::Vector2f lastPosition;
		float lastRotation;
		Math::Vector2f lastScale;

		Math::Vector2f GlobalPosition;
		float GlobalRotation;
		Math::Vector2f GlobalScale;
		
		Math::Matrix3x3f GlobalTransformationMatrix;
		
	public:
		TransformBuffer myTransformBuffer;

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