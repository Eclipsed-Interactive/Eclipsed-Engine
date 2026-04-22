#ifdef ECLIPSED_EDITOR
#include "AbstractContextMenu.h"

#include "ImGui/imgui.h"

#include "EclipsedEngine/Editor/Windows/WindowTypes/HierarchyWindow.h"

#include <filesystem>
#include "CoreEngine/PathManager.h"

namespace Eclipse::Editor
{
	AbstractContextMenu::AbstractContextMenu(const char* name) : ctxName(name)
	{

	}

	void AbstractContextMenu::Draw()
	{
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_Childing_Reordering"))
			{
				IM_ASSERT(payload->DataSize == sizeof(unsigned));
				unsigned draggedEntityID = *reinterpret_cast<unsigned*>(payload->Data);

				std::filesystem::path pathToPlacePrefab = PathManager::GetAssetsPath();
				HierarchyWindow::CreatePrefab(draggedEntityID, pathToPlacePrefab);
			}
			ImGui::EndDragDropTarget();
		}

		if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
		{
			ImGui::OpenPopup(ctxName);
		}

		UpdateAlways();

		if (BeginMenu())
		{
			Update();
			EndMenu();
		}
	}

	bool  AbstractContextMenu::BeginMenu() const
	{
		if (ImGui::BeginPopup(ctxName))
		{
			/*ImGui::FocusPreviousWindowIgnoringOne(ImGui::GetCurrentWindow());*/
			return true;
		}
		return false;
	}

	void AbstractContextMenu::EndMenu() const
	{
		ImGui::EndPopup();
	}
}
#endif