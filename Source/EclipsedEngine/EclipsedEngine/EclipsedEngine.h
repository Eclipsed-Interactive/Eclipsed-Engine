#pragma once

#include "EclipsedEngine.Core.hpp"

namespace Eclipse
{
	namespace Graphics { class IRenderer; }
	namespace Input { class AbstractInput; }

	class ECL_API Engine
	{
	public:
		void Init();
		void LateInit();

		void BeginFrame();
		void EndFrame();
		void Render();
		bool ShouldClose();

	private:
		void ImGui_NewFrame();
		void ImGui_EndFrame();
		void ImGui_Init();
		void ImGui_Render();

	public:
		void* GetImGuiContext();

	private:
		Input::AbstractInput* input;
		Graphics::IRenderer* renderer;
	};
}