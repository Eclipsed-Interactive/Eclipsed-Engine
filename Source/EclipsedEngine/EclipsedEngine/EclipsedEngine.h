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
		static void Init();
		static void LateInit();

		static void BeginFrame();
		static void EndFrame();
		static void Update();
		static void Render();
		static void LateRender();
		static bool ShouldClose();
		static void Shutdown();

		static void UpdateGame();

	private:
		static void ImGui_NewFrame();
		static void ImGui_EndFrame();
		static void ImGui_Init();
		static void ImGui_Render();

		static Graphics::CommandListManager* GetCommandListManager();

	public:
		static void* GetImGuiContext();

	private:
		static Graphics::IRenderer* renderer;
		static Audio::IAudioBackend* audio;

		static Graphics::CommandListManager myCommandListManager;
	};
}