#include "OpenGL_Input.h"

#include "OpenGL/GLFW/glfw3.h"
#include "Core/MainSingleton.h"

#include "Renderer/RendererManager.h"
#include "OpenGL_Renderer/OpenGL_Renderer.h"

namespace Eclipse::OpenGL::Input
{
	OpenGL_Input* GetInputPtrFromGlfwPtr(GLFWwindow* w) {
		return (OpenGL_Input*)glfwGetWindowUserPointer(w);
	}

	void OpenGL_Input::Init()
	{
		Graphics::OpenGL::OpenGL_Renderer& r = (Graphics::OpenGL::OpenGL_Renderer&)Eclipse::Graphics::RendererManager::GetRenderer();
		GLFWwindow* window = r.GetWindow();

		glfwSetWindowUserPointer(window, this);
		
		glfwSetKeyCallback(window, [](GLFWwindow* w, int key, int scancode, int action, int mods) {
			OpenGL_Input* input = GetInputPtrFromGlfwPtr(w);
			if (input) input->OnKey_Callback(w, key, scancode, action, mods);
			});

		glfwSetCursorPosCallback(window, [](GLFWwindow* w, double x, double y) {
			OpenGL_Input* input = GetInputPtrFromGlfwPtr(w);
			if (input) input->OnMousePos_Callback(w, x, y);
			});

		glfwSetCursorEnterCallback(window, [](GLFWwindow* w, int entered) {
			OpenGL_Input* input = GetInputPtrFromGlfwPtr(w);
			if (input) input->OnMouseEnter_Callback(w, entered);
			});

		glfwSetMouseButtonCallback(window, [](GLFWwindow* w, int button, int action, int mods) {
			OpenGL_Input* input = GetInputPtrFromGlfwPtr(w);
			if (input) input->OnMouseButton_Callback(w, button, action, mods);
			});

		glfwSetWindowFocusCallback(window, [](GLFWwindow* w, int focused) {
			OpenGL_Input* input = GetInputPtrFromGlfwPtr(w);
			if (input) input->OnWindowFocus_Callback(w, focused);
			});

		glfwSetScrollCallback(window, [](GLFWwindow* w, double xOffset, double yOffset) {
			OpenGL_Input* input = GetInputPtrFromGlfwPtr(w);
			if (input) input->OnMouseScroll_Callback(w, xOffset, yOffset);
			});
	}
}