#include "Button.h"

#include "EclipsedEngine/Input/Input.h"
#include "EclipsedEngine/Components/UI/RectTransform.h"
#include "CoreEngine/Debug/DebugLogger.h"
#include "EclipsedEngine/Components/UI/Canvas.h"

#include "EclipsedEngine/Editor/Windows/WindowTypes/GameWindow.h"

namespace Eclipse
{
	void Button::Update()
	{
		auto transform = gameObject->GetComponent<RectTransform>();
		if (!transform) return;

		UpdateRectProperties();
		
		if (Input::GetMouseDown(Keycode::MOUSE_LEFT) && IsMouseInsideRect()) {
			OnClickEvent.Invoke();
		}
	}

	void Button::Awake()
	{
		OnClickEvent = Event<>(&Button::Print); 

		UpdateRectProperties();
	}

	bool Button::IsMouseInsideRect()
	{
		auto transform = gameObject->GetComponent<RectTransform>();

		Math::Vector2f refRes = transform->myCanvas->ReferenceResolution.Get();
		
		Math::Vector2f mousePos = Input::GetGameMousePos();
		Math::Vector2f translatedMousePos = mousePos * refRes - refRes * 0.5f;

		std::cout << translatedMousePos.x << "     " << translatedMousePos.y << std::endl;
		
		bool x = translatedMousePos.x < topRightCorner.x && translatedMousePos.x > lowerLeftCorner.x;
		bool y = translatedMousePos.y < topRightCorner.y && translatedMousePos.y > lowerLeftCorner.y;

		return x && y;
	}

	void Button::UpdateRectProperties()
	{
		auto transform = gameObject->GetComponent<RectTransform>();
		if (!transform) return;

		//Math::Vector2f gameResolution = transform->myCanvas->ReferenceResolution.Get();
		//float aspectResoluton = gameResolution.y / gameResolution.x;
		
		Math::Vector2f halfWitdh = transform->WidthHeightPX.Get() * 0.5f;
		topRightCorner = transform->Position.Get() + halfWitdh;
		lowerLeftCorner = transform->Position.Get() - halfWitdh;
	}

	void Button::Print()
	{
		std::cout << "clickButton" << std::endl;
	}
}
