#pragma once

#include "EclipsedEngine.Core.hpp"

#include "Renderer/RenderCommands/CommandList.h"

namespace Eclipse
{
	namespace Graphics { class IRenderer; class CommandListManager; }
	namespace Audio { class IAudioBackend; }
	namespace Input { class AbstractInput; }

	class ECL_API Engine
	{
	public:
		void Init();
		void LateInit();

		void BeginFrame();
		void EndFrame();
		void Update();
		void Render();
		void LateRender();
		bool ShouldClose();

	private:
		void ImGui_NewFrame();
		void ImGui_EndFrame();
		void ImGui_Init();
		void ImGui_Render();

		Graphics::CommandListManager* GetCommandListManager();

	public:
		void* GetImGuiContext();

	private:
		Graphics::IRenderer* renderer = nullptr;
		Audio::IAudioBackend* audio = nullptr;

		Graphics::CommandListManager myCommandListManager;
	};
}