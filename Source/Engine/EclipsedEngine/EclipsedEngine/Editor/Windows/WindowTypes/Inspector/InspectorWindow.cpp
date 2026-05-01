#ifdef ECLIPSED_EDITOR
#include "InspectorWindow.h"

#include "EclipsedEngine/Editor/SelectionContext.h"
#include "EclipsedEngine/Editor/ImGui/ImGui_Impl.h"
#include "Assets/AssetInspectorRegistry.h"
#include "InspectorDispatcher.h"

namespace Eclipse::Editor
{
	void InspectorWindow::Open()
	{
		static bool isInitialized = false;
		if (!isInitialized)
		{
			isInitialized = true;

			AssetInspectorRegistry::RegisterAll();
		}
	}

	void InspectorWindow::Update()
	{
		DrawInspectorHeader();

		if (!lockInspector) currentTarget = SelectionContext::GetCurrentData();

		InspectorDispatcher::Draw(currentTarget);
	}

	void InspectorWindow::DrawInspectorHeader()
	{
		ImGui::SetCursorPosX(ImGui::GetWindowSize().x - 48);
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 6);
		ImGui::Text(ICON_FA_LOCK);
		ImGui::SameLine();
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 6);
		ImGui::Checkbox("##lockinspector", &lockInspector);

		ImGui::Separator();

		ImGui::Spacing();
		ImGui::Spacing();
	}
}
#endif