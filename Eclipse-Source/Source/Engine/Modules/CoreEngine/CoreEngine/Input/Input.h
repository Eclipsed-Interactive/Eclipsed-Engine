#pragma once

#include <bitset>

#include "CoreEngine/Math/Vector/Vector2.h"
#include "Keycodes.h"

struct GLFWwindow;
namespace Eclipse::Core
{
#define MAX_KEYS 512

	class Input
	{
	public:
		bool GetKey(char aKey);
		bool GetKey(int aKey);
		bool GetKey(Keycode::Scancode aKey);

		bool GetKeyDown(char aKey);
		bool GetKeyDown(int aKey);
		bool GetKeyDown(Keycode::Scancode aKey);

		bool GetKeyUp(char aKey);
		bool GetKeyUp(int aKey);
		bool GetKeyUp(Keycode::Scancode aKey);

		bool GetAny();
		Keycode::Scancode GetAnyKey();

		bool GetMouse(int aKey);
		bool GetMouse(Keycode::Scancode aKey);

		bool GetMouseDown(int aKey);
		bool GetMouseDown(Keycode::Scancode aKey);

		bool GetMouseUp(int aKey);
		bool GetMouseUp(Keycode::Scancode aKey);


		const Math::Vector2i& GetGameMousePos();

		const Math::Vector2i& GetMousePos();
		const Math::Vector2i& GetMouseDeltaPos();

		const Math::Vector2i& GetScroll();
		const Math::Vector2i& GetNormalizedScroll();


		bool IsWindowFocused();
		bool IsMouseInside();

	public:
		void Init();
		void Update();

		// This is used when in editor
		void SetGamePosition(const Math::Vector2i& aPosition);

	private:
		void OnKey_Callback(GLFWwindow* window, int key, int scancode, int action, int mods);
		void OnMousePos_Callback(GLFWwindow* window, double x, double y);
		void OnMouseEnter_Callback(GLFWwindow* window, int entered);
		void OnMouseButton_Callback(GLFWwindow* window, int button, int action, int mods);
		void OnWindowFocus_Callback(GLFWwindow* window, int focused);
		void OnMouseScroll_Callback(GLFWwindow* window, double xOffset, double yOffset);

	private:
		std::bitset<MAX_KEYS> currentKeys;
		std::bitset<MAX_KEYS> lastKeys;

		std::bitset<MAX_KEYS> pressedThisFrame;
		std::bitset<MAX_KEYS> releasedThisFrame;

		Math::Vector2i currentGamePos;

		Math::Vector2i currentPos;
		Math::Vector2i lastPos;
		Math::Vector2i mouseDeltaPos;

		Math::Vector2i mouseScrollDelta;
		Math::Vector2i normalizedMouseScrollDelta;

		bool mouseIsInside;
		bool windowFocused;
	};
}