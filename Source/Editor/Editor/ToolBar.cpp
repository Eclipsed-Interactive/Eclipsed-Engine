#include "ToolBar.h"

#include "Views/Types/AssetBrowserView.h"
#include "Views/Types/Inspector/InspectorView.h"
#include "Views/Types/GameView.h"
#include "Views/Types/SceneView.h"
#include "Views/Types/HierarchyView.h"
#include "Views/Types/SettingsView.h"

#include <regex>
#include "ImGui/imgui.h"
#include "Core/Settings/EditorSettings.h"

#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"
#include "FontAwesome/7/IconsFontAwesome7.h"

namespace Eclipse::Editor
{
	void ToolBar::Draw()
	{
		static PlayMode mode = PlayMode::Stopped;

		ImGuiViewport* viewport = ImGui::GetMainViewport();

		ImVec2 workPos = viewport->WorkPos;
		ImVec2 workSize = viewport->WorkSize;

		ImGui::SetNextWindowPos(
			ImVec2(
				workPos.x,
				workPos.y
			)
		);

		ImGui::SetNextWindowSize(
			ImVec2(
				workSize.x,
				ToolBarHeight
			)
		);

		ImGuiWindowFlags flags =
			ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoSavedSettings |
			ImGuiWindowFlags_NoDocking |
			ImGuiWindowFlags_NoScrollbar;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(6.0f, 4.0f));

		ImGui::Begin("##EditorToolbar", nullptr, flags);

		switch (mode)
		{
		case PlayMode::Playing: DrawPlayMode(mode);
			break;
		case PlayMode::Paused: DrawPausedMode(mode);
			break;
		case PlayMode::Stopped: DrawStoppedMode(mode);
			break;
		}

		ImGui::End();

		ImGui::PopStyleVar(3);
	}

	void ToolBar::DrawPlayMode(PlayMode& currentMode)
	{
		ImGui::SameLine((ImGui::GetWindowWidth() - 25) * 0.5f - PlayModeButtonsPadding);
		if (ImGui::Button(ICON_FA_PAUSE, PlayModeButtonSize))
		{
			currentMode = PlayMode::Paused;
		}

		ImGui::SameLine((ImGui::GetWindowWidth() - 25) * 0.5f + PlayModeButtonsPadding);
		if (ImGui::Button(ICON_FA_STOP, PlayModeButtonSize))
		{
			currentMode = PlayMode::Stopped;
		}
	}

	void ToolBar::DrawPausedMode(PlayMode& currentMode)
	{
		ImGui::SameLine((ImGui::GetWindowWidth() - 25) * 0.5f - PlayModeButtonsPadding);
		if (ImGui::Button(ICON_FA_PLAY, PlayModeButtonSize))
		{
			currentMode = PlayMode::Playing;
		}

		ImGui::SameLine((ImGui::GetWindowWidth() - 25) * 0.5f + PlayModeButtonsPadding);
		if (ImGui::Button(ICON_FA_STOP, PlayModeButtonSize))
		{
			currentMode = PlayMode::Stopped;
		}
	}

	void ToolBar::DrawStoppedMode(PlayMode& currentMode)
	{
		ImGui::SameLine((ImGui::GetWindowWidth() - 25) * 0.5f);
		if (ImGui::Button(ICON_FA_PLAY, PlayModeButtonSize))
		{
			currentMode = PlayMode::Playing;
		}
	}
}