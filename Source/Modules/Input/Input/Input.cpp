#include "Input.h"

#include "Core/MainSingleton.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"

namespace Eclipse::Input
{
	AbstractInput* Input::input = nullptr;

#ifdef ECL_EDITOR
	std::vector<std::string> Input::gameViewWindows;

	Math::Vector2ui Input::GetActiveGameViewMousePos()
	{
		ImVec2 mouse = ImGui::GetMousePos();

		for (const std::string& id : gameViewWindows)
		{
			ImGuiWindow* window = ImGui::FindWindowByName(id.c_str());

			if (!window)
				continue;

			if (mouse.x >= window->Pos.x &&
				mouse.x <= window->Pos.x + window->Size.x &&
				mouse.y >= window->Pos.y &&
				mouse.y <= window->Pos.y + window->Size.y)
			{
				return Math::Vector2ui(
					mouse.x - window->Pos.x,
					mouse.y - window->Pos.y
				);
			}
		}

		return Math::Vector2ui(0, 0);
	}
#endif

	void Input::Init(AbstractInput* inputInstance, void* imguiCtx)
	{
		ImGui::SetCurrentContext((ImGuiContext*)imguiCtx);

		input = inputInstance;
		input->Init();
	}

	void Input::Update()
	{
		input->Update();
	}

	bool Input::GetKey(char aKey)
	{
		return input->GetKey(aKey);
	}

	bool Input::GetKey(int aKey)
	{
		return input->GetKey(aKey);
	}

	bool Input::GetKey(Keycode::Scancode aKey)
	{
		return input->GetKey(aKey);
	}

	bool Input::GetKeyDown(char aKey)
	{
		return input->GetKeyDown(aKey);
	}

	bool Input::GetKeyDown(int aKey)
	{
		return input->GetKeyDown(aKey);
	}

	bool Input::GetKeyDown(Keycode::Scancode aKey)
	{
		return input->GetKeyDown(aKey);
	}

	bool Input::GetKeyUp(char aKey)
	{
		return input->GetKeyDown(aKey);
	}

	bool Input::GetKeyUp(int aKey)
	{
		return input->GetKeyUp(aKey);
	}

	bool Input::GetKeyUp(Keycode::Scancode aKey)
	{
		return input->GetKeyUp(aKey);
	}

	bool Input::GetAny()
	{
		return input->GetAny();
	}

	Keycode::Scancode Input::GetAnyKey()
	{
		return input->GetAnyKey();
	}

	bool Input::GetMouse(int aKey)
	{
		return input->GetMouse(aKey);
	}

	bool Input::GetMouse(Keycode::Scancode aKey)
	{
		return input->GetMouse(aKey);
	}

	bool Input::GetMouseDown(int aKey)
	{
		return input->GetMouseDown(aKey);
	}

	bool Input::GetMouseDown(Keycode::Scancode aKey)
	{
		return input->GetMouseDown(aKey);
	}

	bool Input::GetMouseUp(int aKey)
	{
		return input->GetMouseUp(aKey);
	}

	bool Input::GetMouseUp(Keycode::Scancode aKey)
	{
		return input->GetMouseUp(aKey);
	}

	const Math::Vector2i& Input::GetMousePos()
	{
#ifdef ECL_EDITOR
		Math::Vector2ui pos = GetActiveGameViewMousePos();
		return pos;
#else
		return input->GetMousePos();
#endif
	}

	const Math::Vector2i& Input::GetMouseDeltaPos()
	{
		return input->GetMouseDeltaPos();
	}

	const Math::Vector2i& Input::GetScroll()
	{
		return input->GetScroll();
	}

	const Math::Vector2i& Input::GetNormalizedScroll()
	{
		return input->GetNormalizedScroll();
	}

	bool Input::IsWindowFocused()
	{
		return input->IsWindowFocused();
	}

	bool Input::IsMouseInside()
	{
		return input->IsMouseInside();
	}
}