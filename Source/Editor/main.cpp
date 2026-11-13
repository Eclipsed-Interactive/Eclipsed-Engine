#include <iostream>
#include <Windows.h>

#include "EclipsedEngine/EclipsedEngine.h"
#include "Editor/Editor.h"

#include "ImGui/imgui.h"

#include <Core/Profiling/ProfilerManager.h>
#include <Core/Profiling/ProfilerMacros.hpp>

using namespace Eclipse::Profiler;

void Test_ImGui();
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	Eclipse::Editor::Editor editor;

	ProfilerManager::BeginPhase(Phase::Init);
	{
		PROFILE_SCOPED;
		editor.EarlyInit();

		editor.Init();
	}
	ProfilerManager::EndPhase();

	while (!editor.ShouldClose())
	{
		ProfilerManager::BeginPhase(Phase::Update);
		{
			PROFILE_SCOPED;
			editor.BeginFrame();

			Test_ImGui();

			editor.Update();
			editor.Render();
		}

		ProfilerManager::EndPhase();

		editor.EndFrame();
	}

	editor.Shutdown();
}

// Write imgui stuff to test here.
// New windows or testing.
// Just remember to remove before pushing to git.
void Test_ImGui()
{
	//ImGui::Begin("Simons window");
	//ImGui::End();

	//ImGui::Begin("Testing");
	//ImGui::End();
}
