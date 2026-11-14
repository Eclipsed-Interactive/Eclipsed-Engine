#include "GraphicsSettingsPanel.h"

#include <Renderer/GraphicsSettings.h>
#include "ImGui/imgui.h"

namespace Eclipse::Editor
{
	void GraphicsSettingsPanel::Draw()
	{
		Settings::GraphicsSettings& settings = Settings::GetGraphicsSettings();

		ImGui::Text("Render Lights");
		ImGui::SameLine();
		ImGui::Checkbox("##render-lights", &settings.renderLights);

		ImGui::Text("Number of Render buffers");
		ImGui::SameLine();
		ImGui::DragInt("##numberofbuffers", &settings.numberOfRenderBuffers);
	}
}
