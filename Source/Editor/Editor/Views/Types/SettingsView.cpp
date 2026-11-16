#include "SettingsView.h"

#include "Input/Input.h"
#include "Input/InputMapper.h"
#include "ImGui/imgui.h"

#include <algorithm>
#include <string>
#include <vector>

#include <sstream>

namespace Eclipse::Editor
{
	void SettingsView::OnOpen()
	{
		inputSettings.OnOpen();
		//graphicsSettings.
	}

	void SettingsView::Draw()
	{
		if (ImGui::CollapsingHeader("Input"))
		{
			ImGui::Spacing();

			Indent(10);

			inputSettings.Draw();

			Unindent(10);
		}

		if (ImGui::CollapsingHeader("Graphics"))
		{
			ImGui::Spacing();

			Indent(10);

			graphicsSettings.Draw();

			Unindent(10);
		}

		if (ImGui::CollapsingHeader("Editor"))
		{
			ImGui::Spacing();

			Indent(10);

			editorSettings.Draw();

			Unindent(10);
		}
	}
}