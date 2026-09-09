#pragma once

#include "OpenGL_Renderer.Core.hpp"
#include "Input/AbstractInput.h"

struct GLFWwindow;
namespace Eclipse::OpenGL::Input
{
#define MAX_KEYS 512

	class OpenGL_Input : public Eclipse::Input::AbstractInput
	{
	public:
		void Init() override;

	private:
		void OnKey_Callback(GLFWwindow* window, int key, int scancode, int action, int mods);
		void OnMousePos_Callback(GLFWwindow* window, double x, double y);
		void OnMouseEnter_Callback(GLFWwindow* window, int entered);
		void OnMouseButton_Callback(GLFWwindow* window, int button, int action, int mods);
		void OnWindowFocus_Callback(GLFWwindow* window, int focused);
		void OnMouseScroll_Callback(GLFWwindow* window, double xOffset, double yOffset);
	};
}