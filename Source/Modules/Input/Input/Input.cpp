#include "Input.h"

#include "OpenGL/GLFW/glfw3.h"
#include "Core/MainSingleton.h"

namespace Eclipse::Input
{
	std::bitset<MAX_KEYS> Input::currentKeys;
	std::bitset<MAX_KEYS> Input::lastKeys;

	std::bitset<MAX_KEYS> Input::pressedThisFrame;
	std::bitset<MAX_KEYS> Input::releasedThisFrame;

	Math::Vector2f Input::currentGamePos;

	Math::Vector2i Input::currentPos;
	Math::Vector2i Input::lastPos;
	Math::Vector2i Input::mouseDeltaPos;

	Math::Vector2i Input::mouseScrollDelta;
	Math::Vector2i Input::normalizedMouseScrollDelta;

	bool Input::mouseIsInside;
	bool Input::windowFocused;

	Input* GetInputPtrFromGlfwPtr(GLFWwindow* w) {
		return (Input*)glfwGetWindowUserPointer(w);
	}

	void Input::Update()
	{

		// Mouse delta
		//mouseScrollDelta = Math::Vector2i(0, 0);
		normalizedMouseScrollDelta = Math::Vector2i(0, 0);

		// Mouse pos 
		mouseDeltaPos = currentPos - lastPos;
		lastPos = currentPos;

		// Buttons (keyboard and mouse buttons)
		pressedThisFrame = currentKeys & ~lastKeys;
		releasedThisFrame = ~currentKeys & lastKeys;

		lastKeys = currentKeys;

		if (pressedThisFrame[Keycode::SPACE])
		{
			return;
		}
	}

	void Input::Init()
	{
		GLFWwindow* window = MainSingleton::GetInstance<GLFWwindow*>();
		//glfwSetWindowUserPointer(window, this);

		glfwSetKeyCallback(window, [](GLFWwindow* w, int key, int scancode, int action, int mods) {
			Input::OnKey_Callback(w, key, scancode, action, mods);
			});

		glfwSetCursorPosCallback(window, [](GLFWwindow* w, double x, double y) {
			Input::OnMousePos_Callback(w, x, y);
			});

		glfwSetCursorEnterCallback(window, [](GLFWwindow* w, int entered) {
			Input::OnMouseEnter_Callback(w, entered);
			});

		glfwSetMouseButtonCallback(window, [](GLFWwindow* w, int button, int action, int mods) {
			Input::OnMouseButton_Callback(w, button, action, mods);
			});

		glfwSetWindowFocusCallback(window, [](GLFWwindow* w, int focused) {
			Input::OnWindowFocus_Callback(w, focused);
			});

		glfwSetScrollCallback(window, [](GLFWwindow* w, double xOffset, double yOffset) {
			Input::OnMouseScroll_Callback(w, xOffset, yOffset);
			});
	}
}