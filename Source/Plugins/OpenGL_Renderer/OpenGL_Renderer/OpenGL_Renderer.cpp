#include "OpenGL_Renderer.h"

#include "OpenGL/glad/glad.h"
#include "OpenGL/GLFW/glfw3.h"

#define GLFW_EXPOSE_NATIVE_WIN32
#include "OpenGL/ImGui/imgui_impl_glfw.h"
#include "OpenGL/ImGui/imgui_impl_opengl3.h"

#include "Core/MainSingleton.h"

#include "Input/OpenGL_Input.h"

#include "OpenGL_GraphicsBuffer.h"
#include "OpenGL_UniformVariableManager.h"
#include "OpenGL_Drawer.h"
#include "OpenGL_GraphicsDevice.h"

namespace Eclipse::Graphics::OpenGL
{
	ErrorCode OpenGL_Renderer::Init()
	{
		if (!glfwInit())
		{
			return ErrorCode{};
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		float width = 1280;
		float height = 720;

		window = glfwCreateWindow(width, height, "Eclipsed Game Engine", nullptr, nullptr);

		if (!window)
		{
			glfwTerminate();
			return ErrorCode{};
		}

		glfwMakeContextCurrent(window);
		glfwSwapInterval(1);

		gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

		device = new OpenGL_GraphicsDevice;
		graphicsBuffer = new OpenGL_GraphicsBuffer;
		uniformVariableManager = new OpenGL_UniformVariableManager;
		drawer = new OpenGL_Drawer;

		drawer->mySprite = device->CreateSprite();

		return ErrorCode{};
	}

	void OpenGL_Renderer::BeginFrame()
	{
		glfwPollEvents();

		glClearColor(0.3f, 0.3f, 0.3f, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGL_Renderer::EndFrame()
	{
		glfwSwapBuffers(window);
	}

	bool OpenGL_Renderer::ShouldWindowClose()
	{
		return glfwWindowShouldClose(window);
	}

	void OpenGL_Renderer::Render()
	{

	}

	Input::AbstractInput* OpenGL_Renderer::CreateInput()
	{
		using namespace Eclipse::OpenGL::Input;
		return new OpenGL_Input;
	}

	GLFWwindow* OpenGL_Renderer::GetWindow()
	{
		return window;
	}
	void OpenGL_Renderer::ImGui_NewFrame()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
	}

	void OpenGL_Renderer::ImGui_Init(void* imguiCtx)
	{
		ImGui::SetCurrentContext((ImGuiContext*)imguiCtx);

		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 460");
	}
	
	void OpenGL_Renderer::ImGui_Render()
	{
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup = glfwGetCurrentContext();

			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();

			glfwMakeContextCurrent(backup);
		}
	}
}