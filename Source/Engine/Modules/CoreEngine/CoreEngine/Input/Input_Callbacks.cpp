#include "pch.h"

#include "Input.h"

#include "OpenGL/GLFW/glfw3.h"
#include <algorithm>

namespace Eclipse::Core
{
	void Input::OnMousePos_Callback(GLFWwindow* w, double x, double y)
	{
		currentPos.x = static_cast<int>(x);
		currentPos.y = static_cast<int>(y);
	}

	void Input::OnMouseEnter_Callback(GLFWwindow* w, int entered)
	{
		mouseIsInside = (bool)entered;
	}

	void Input::OnMouseButton_Callback(GLFWwindow* w, int button, int action, int mods)
	{
		if (action == GLFW_PRESS) currentKeys[button] = true;
		else if (action == GLFW_RELEASE) currentKeys[button] = false;
	}

	void Input::OnWindowFocus_Callback(GLFWwindow* w, int focused)
	{
		windowFocused = focused;
	}

	void Input::OnMouseScroll_Callback(GLFWwindow* w, double xOffset, double yOffset)
	{
		mouseScrollDelta.x += xOffset;
		mouseScrollDelta.y += yOffset;

		normalizedMouseScrollDelta.x = std::clamp(mouseScrollDelta.x, -1, 1);
		normalizedMouseScrollDelta.y = std::clamp(mouseScrollDelta.y, -1, 1);
	}

	void Input::OnKey_Callback(GLFWwindow* w, int key, int scancode, int action, int mods)
	{
		if (key < 0 || key >= MAX_KEYS) return;

		if (action == GLFW_PRESS) currentKeys[key] = true;
		else if (action == GLFW_RELEASE) currentKeys[key] = false;
	}
}