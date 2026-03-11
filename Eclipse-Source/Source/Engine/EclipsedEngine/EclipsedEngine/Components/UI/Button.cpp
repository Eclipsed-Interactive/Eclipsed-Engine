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
		Math::Vector2i mousePos = Input::GetGameMousePos();

		bool x = mousePos.x < topRightCorner.x && mousePos.x > lowerLeftCorner.x;
		bool y = mousePos.y < topRightCorner.y && mousePos.y > lowerLeftCorner.y;

		return x && y;
	}

	void Button::UpdateRectProperties()
	{
		auto transform = gameObject->GetComponent<RectTransform>();
		if (!transform) return;

		Math::Vector2f resolution = transform->myCanvas->ReferenceResolution;
		float aspectRatio = resolution.x / resolution.y;
		Math::Vector2f canvasScaleRelationOneDiv = { 1.f / resolution.x, 1.f / resolution.y };


		Math::Vector2f gameResolution = Editor::GameWindow::myGameImageResolution;
		Math::Vector2f halfWitdh = transform->WidthHeightPX.Get() * 0.5f;
		halfWitdh *= Math::Vector2f(0.5f, 0.5f);
		topRightCorner = transform->Position.Get() + halfWitdh + (gameResolution * 0.5f);
		lowerLeftCorner = transform->Position.Get() - halfWitdh + (gameResolution * 0.5f);
	}

	void Button::Print()
	{
		Replication::ReplicationManager::Start();
	}
}
