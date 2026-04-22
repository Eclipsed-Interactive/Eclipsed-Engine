#include "pch.h"

#include "Input.h"

#include "OpenGL/GLFW/glfw3.h"

namespace Eclipse::Core
{
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
	}

	void Input::Init()
	{
		GLFWwindow* window = MainSingleton::GetInstance<GLFWwindow*>();
		glfwSetWindowUserPointer(window, this);

		glfwSetKeyCallback(window, [](GLFWwindow* w, int key, int scancode, int action, int mods) {
			Input* input = GetInputPtrFromGlfwPtr(w);
			if (input) input->OnKey_Callback(w, key, scancode, action, mods);
			});

		glfwSetCursorPosCallback(window, [](GLFWwindow* w, double x, double y) {
			Input* input = GetInputPtrFromGlfwPtr(w);
			if (input) input->OnMousePos_Callback(w, x, y);
			});

		glfwSetCursorEnterCallback(window, [](GLFWwindow* w, int entered) {
			Input* input = GetInputPtrFromGlfwPtr(w);
			if (input) input->OnMouseEnter_Callback(w, entered);
			});

		glfwSetMouseButtonCallback(window, [](GLFWwindow* w, int button, int action, int mods) {
			Input* input = GetInputPtrFromGlfwPtr(w);
			if (input) input->OnMouseButton_Callback(w, button, action, mods);
			});

		glfwSetWindowFocusCallback(window, [](GLFWwindow* w, int focused) {
			Input* input = GetInputPtrFromGlfwPtr(w);
			if (input) input->OnWindowFocus_Callback(w, focused);
			});

		glfwSetScrollCallback(window, [](GLFWwindow* w, double xOffset, double yOffset) {
			Input* input = GetInputPtrFromGlfwPtr(w);
			if (input) input->OnMouseScroll_Callback(w, xOffset, yOffset);
			});
	}
}