#include "ToolBar.h"

#include "Views/Types/AssetBrowserView.h"
#include "Views/Types/Inspector/InspectorView.h"
#include "Views/Types/GameView.h"
#include "Views/Types/SceneView.h"
#include "Views/Types/HierarchyView.h"
#include "Views/Types/SettingsView.h"

#include <regex>
#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"
#include "Core/Settings/EditorSettings.h"

#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"
#include "FontAwesome/7/IconsFontAwesome7.h"

#include <EclipsedEngine/EclipsedEngine.h>
#include <Editor/PlayModeController.h>

namespace Eclipse::Editor
{
	void ToolBar::Draw()
	{
		ImGuiWindowFlags flags =
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoSavedSettings |
			ImGuiWindowFlags_NoScrollbar |
			ImGuiWindowFlags_NoScrollWithMouse;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(6.0f, 4.0f));

		if (ImGui::BeginViewportSideBar("##EditorToolbar", ImGui::GetMainViewport(), ImGuiDir_Up, ToolBarHeight, flags))
		{
			switch (PlayModeController::GetPlayMode())
			{
			case PlayMode::Playing: DrawPlayMode();
				break;
			case PlayMode::Paused: DrawPausedMode();
				break;
			case PlayMode::Stopped: DrawStoppedMode();
				break;
			}
		}
		ImGui::End();

		ImGui::PopStyleVar(2);
	}

	void ToolBar::CenterButtons(int buttonCount)
	{
		const float groupWidth = buttonCount * PlayModeButtonSize.x + (buttonCount - 1) * PlayModeButtonSpacing;

		ImGui::SetCursorPos(ImVec2(
			(ImGui::GetWindowWidth() - groupWidth) * 0.5f,
			(ImGui::GetWindowHeight() - PlayModeButtonSize.y) * 0.5f));
	}

	void ToolBar::DrawPlayMode()
	{
		CenterButtons(2);

		if (ImGui::Button(ICON_FA_PAUSE, PlayModeButtonSize))
		{
			PlayModeController::SetPlayMode(PlayMode::Paused);
		}

		ImGui::SameLine(0.f, PlayModeButtonSpacing);
		if (ImGui::Button(ICON_FA_STOP, PlayModeButtonSize))
		{
			PlayModeController::SetPlayMode(PlayMode::Stopped);
		}
	}

	void ToolBar::DrawPausedMode()
	{
		CenterButtons(3);

		if (ImGui::Button(ICON_FA_PLAY, PlayModeButtonSize))
		{
			PlayModeController::SetPlayMode(PlayMode::Playing);
		}

		ImGui::SameLine(0.f, PlayModeButtonSpacing);
		if (ImGui::Button(ICON_FA_STOP, PlayModeButtonSize))
		{
			PlayModeController::SetPlayMode(PlayMode::Stopped);
		}

		ImGui::SameLine(0.f, PlayModeButtonSpacing);
		if (ImGui::Button(ICON_FA_FORWARD_STEP, PlayModeButtonSize))
		{
			PlayModeController::Step();
		}
	}

	void ToolBar::DrawStoppedMode()
	{
		CenterButtons(1);

		if (ImGui::Button(ICON_FA_PLAY, PlayModeButtonSize))
		{
			PlayModeController::SetPlayMode(PlayMode::Playing);
		}
	}
}