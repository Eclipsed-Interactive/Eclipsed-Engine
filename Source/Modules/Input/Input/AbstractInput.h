#pragma once

#include "Input.Core.hpp"

#include <bitset>

#include "Core/Math/Vector/Vector2.h"
#include "Keycodes.h"

namespace Eclipse::Input
{
#define MAX_KEYS 512
	class INPUT_API AbstractInput
	{
	public:
		virtual bool GetKey(char aKey) { return false; }
		virtual bool GetKey(int aKey) { return false; }
		virtual bool GetKey(Keycode::Scancode aKey) { return false; }

		virtual bool GetKeyDown(char aKey) { return false; }
		virtual bool GetKeyDown(int aKey) { return false; }
		virtual bool GetKeyDown(Keycode::Scancode aKey) { return false; }

		virtual bool GetKeyUp(char aKey) { return false; }
		virtual bool GetKeyUp(int aKey) { return false; }
		virtual bool GetKeyUp(Keycode::Scancode aKey) { return false; }

		virtual bool GetAny() { return false; }
		Keycode::Scancode GetAnyKey();

		virtual bool GetMouse(int aKey) { return false; }
		virtual bool GetMouse(Keycode::Scancode aKey) { return false; }

		virtual bool GetMouseDown(int aKey) { return false; }
		virtual bool GetMouseDown(Keycode::Scancode aKey) { return false; }

		virtual bool GetMouseUp(int aKey) { return false; }
		virtual bool GetMouseUp(Keycode::Scancode aKey) { return false; }


		virtual const Math::Vector2f& GetGameMousePos() {return Math::Vector2f(FLT_MIN, FLT_MIN);}

		virtual const Math::Vector2i& GetMousePos() {return Math::Vector2f(FLT_MIN, FLT_MIN);}
		virtual const Math::Vector2i& GetMouseDeltaPos() {return Math::Vector2f(FLT_MIN, FLT_MIN);}

		virtual const Math::Vector2i& GetScroll() {return Math::Vector2f(FLT_MIN, FLT_MIN);}
		virtual const Math::Vector2i& GetNormalizedScroll() { return Math::Vector2f(FLT_MIN, FLT_MIN); }


		virtual bool IsWindowFocused() { return false; }
		virtual bool IsMouseInside() { return false; }

		virtual void SetMousePositionEditor(const Math::Vector2f& aPosition) {}

	public:
		virtual void Init() = 0;

	public:
		void Update();

	protected:
		std::bitset<MAX_KEYS> currentKeys;
		std::bitset<MAX_KEYS> lastKeys;

		std::bitset<MAX_KEYS> pressedThisFrame;
		std::bitset<MAX_KEYS> releasedThisFrame;

		Math::Vector2f currentGamePos;

		Math::Vector2i currentPos;
		Math::Vector2i lastPos;
		Math::Vector2i mouseDeltaPos;

		Math::Vector2i mouseScrollDelta;
		Math::Vector2i normalizedMouseScrollDelta;

		bool mouseIsInside;
		bool windowFocused;
	};
}