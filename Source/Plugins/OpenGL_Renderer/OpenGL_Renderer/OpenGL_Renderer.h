#pragma once

#include "OpenGL_Renderer.Core.hpp"
#include "Renderer/IRenderer.h"

struct GLFWwindow;

namespace Eclipse::Graphics::OpenGL
{
	class OPENGL_RENDERER_API OpenGL_Renderer final : public IRenderer
	{
	public:
		ErrorCode Init() override;

		void BeginFrame() override;
		void EndFrame() override;

		bool ShouldWindowClose() override;

		void Render() override;

		Input::AbstractInput* CreateInput() override;

		GLFWwindow* GetWindow();

	public:
		void ImGui_NewFrame() override;
		void ImGui_Init(void* imguiCtx) override;
		void ImGui_Render() override;

	private:
		GLFWwindow* window;
	};
}