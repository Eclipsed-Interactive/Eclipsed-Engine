#ifdef ECL_EDITOR
#include "InspectorView.h"

#include "Editor/SelectionContext.h"
#include "Editor/ImGui/ImGui_Impl.h"
#include "Assets/AssetInspectorRegistry.h"
#include "InspectorDispatcher.h"

#include "ImGui/ImGui.h"

#include "FontAwesome/7/IconsFontAwesome7.h"

namespace Eclipse::Editor
{
	void InspectorView::OnOpen()
	{
		static bool isInitialized = false;
		if (!isInitialized)
		{
			isInitialized = true;

			AssetInspectorRegistry::RegisterAll();
		}
	}

	void InspectorView::Draw()
	{
		DrawInspectorHeader();

		if (!lockInspector) currentTarget = SelectionContext::GetCurrentData();

		InspectorDispatcher::Draw(currentTarget);
	}

	void InspectorView::DrawInspectorHeader()
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