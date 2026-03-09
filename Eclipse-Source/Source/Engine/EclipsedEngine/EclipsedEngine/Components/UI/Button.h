#pragma once

#include "EclipsedEngine/Components/Component.h"
#include "EntityEngine/ComponentManager.h"

#include "EntityEngine/ComponentManager.h"

#include "CoreEngine/EventSystem/EventSystem.h"


namespace Eclipse
{
	class Button : public Component
	{
		COMPONENT_BASE_2(Button, 0)

	public:
		void Update() override;
		void Awake() override;

	private:
		bool IsMouseInsideRect();
		void UpdateRectProperties();

		static void Print();

	public:
		Event<> OnClickEvent;

	private:
		Math::Vector2f lowerLeftCorner;
		Math::Vector2f topRightCorner;
	};
}