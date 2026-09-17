#include <iostream>
#include <Windows.h>

#include "EclipsedEngine/EclipsedEngine.h"
#include "Editor/Editor.h"

#include "ImGui/imgui.h"

void Test_ImGui();
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	Eclipse::Editor::Editor editor;
	Eclipse::Engine engine;
	editor.EarlyInit();
	engine.Init();

	editor.Init(engine.GetImGuiContext());

	engine.LateInit();

	while (!engine.ShouldClose())
	{
		engine.BeginFrame();
		editor.BeginFrame();

		Test_ImGui();

		engine.Update();

		editor.Render();
		engine.Render();


		editor.EndFrame();
		engine.EndFrame();
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
