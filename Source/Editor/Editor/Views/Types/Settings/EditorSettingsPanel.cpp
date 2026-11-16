#include "EditorSettingsPanel.h"

#include <Editor/EditorSettings.h>
#include "ImGui/imgui.h"

namespace Eclipse::Editor
{
	void EditorSettingsPanel::Draw()
	{
		Settings::EditorSettings& settings = Settings::GetEditorSettings();

		ImGui::Text("Open last active scene on startup");
		ImGui::SameLine();
		ImGui::Checkbox("##openlastactivesceneonstartup", &settings.openLastActiveSceneOnStartup);
	}
}
