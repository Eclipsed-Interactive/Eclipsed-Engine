#include "MainMenuBar.h"

#include "ImGui/imgui.h"
#include "EditorLayout.h"

namespace Eclipse::Editor
{
	void MainMenuBar::Init()
	{

	}

	void MainMenuBar::Draw()
	{
		ImGui::BeginMainMenuBar();

		DrawViews();

		ImGui::EndMainMenuBar();
	}

	void MainMenuBar::DrawViews()
	{
		if (ImGui::BeginMenu("Views"))
		{
			auto views = EditorLayout::GetViewRegistry().GetViewTypes();
			for (auto view : views)
			{
				if (ImGui::MenuItem(view->name.c_str()))
				{
					EditorLayout::OpenView(view->name);
				}
			}

			ImGui::EndMenu();
		}

		if (ImGui::Button("Load 1"))
		{
			EditorLayout::OpenLayout("1");
		}

		if (ImGui::Button("Load 2"))
		{
			EditorLayout::OpenLayout("2");
		}

		if (ImGui::Button("Load 3"))
		{
			EditorLayout::OpenLayout("3");
		}

		ImGui::Dummy({ 100, 0 });

		if (ImGui::Button("Save 1"))
		{
			ImGui::SaveIniSettingsToDisk("C:/Users/zulto/Desktop/MyFiles/Projects/Project-Nova/Project/Editor/Layouts/1.layout");
		}

		if (ImGui::Button("Save 2"))
		{
			ImGui::SaveIniSettingsToDisk("C:/Users/zulto/Desktop/MyFiles/Projects/Project-Nova/Project/Editor/Layouts/2.layout");
		}

		if (ImGui::Button("Save 3"))
		{
			ImGui::SaveIniSettingsToDisk("C:/Users/zulto/Desktop/MyFiles/Projects/Project-Nova/Project/Editor/Layouts/3.layout");
		}

		ImGui::Dummy({ 100, 0 });

		if (ImGui::Button("Save New 1"))
		{
			EditorLayout::SaveNewLayout("1");
		}

		if (ImGui::Button("Save New 2"))
		{
			EditorLayout::SaveNewLayout("2");
		}
	}
}