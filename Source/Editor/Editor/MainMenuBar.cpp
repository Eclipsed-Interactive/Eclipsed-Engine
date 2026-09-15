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
	}
}