#include "DockSpace.h"
#include "ImGui/imgui.h"

#include "ToolBar.h"

namespace Eclipse::Editor
{
	void DockSpace::Draw()
	{
		ImGuiViewport* viewport = ImGui::GetMainViewport();

		ImVec2 workPos = viewport->WorkPos;
		ImVec2 workSize = viewport->WorkSize;

		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(workSize);
		ImGui::SetNextWindowViewport(viewport->ID);

		ImGuiWindowFlags flags =
			ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoBringToFrontOnFocus |
			ImGuiWindowFlags_NoNavFocus |
			ImGuiWindowFlags_NoSavedSettings |
			ImGuiWindowFlags_NoDocking |
			ImGuiWindowFlags_NoScrollbar;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

		ImGui::Begin("##EditorDockSpaceHost", nullptr, flags);

		ImGuiID dockspaceID = ImGui::GetID("EditorDockSpace");
		ImGui::DockSpace(dockspaceID, ImVec2(0, 0), ImGuiDockNodeFlags_PassthruCentralNode);

		ImGui::End();
		ImGui::PopStyleVar(3);
	}
}