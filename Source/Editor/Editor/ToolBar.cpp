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

#include <EclipsedEngine/EclipsedEngine.h>
#include <Editor/PlayModeController.h>

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

		switch (PlayModeController::GetPlayMode())
		{
		case PlayMode::Playing: DrawPlayMode();
			break;
		case PlayMode::Paused: DrawPausedMode();
			break;
		case PlayMode::Stopped: DrawStoppedMode();
			break;
		}

		ImGui::End();

		ImGui::PopStyleVar(3);
	}

	void ToolBar::DrawPlayMode()
	{
		ImGui::SameLine((ImGui::GetWindowWidth() - 25) * 0.5f - PlayModeButtonsPadding);
		if (ImGui::Button(ICON_FA_PAUSE, PlayModeButtonSize))
		{
			PlayModeController::SetPlayMode(PlayMode::Paused);
		}

		ImGui::SameLine((ImGui::GetWindowWidth() - 25) * 0.5f + PlayModeButtonsPadding);
		if (ImGui::Button(ICON_FA_STOP, PlayModeButtonSize))
		{
			PlayModeController::SetPlayMode(PlayMode::Stopped);
		}
	}

	void ToolBar::DrawPausedMode()
	{
		ImGui::SameLine((ImGui::GetWindowWidth() - 25) * 0.5f - (PlayModeButtonsPadding * 2));
		if (ImGui::Button(ICON_FA_PLAY, PlayModeButtonSize))
		{
			PlayModeController::SetPlayMode(PlayMode::Playing);
		}

		ImGui::SameLine((ImGui::GetWindowWidth() - 25) * 0.5f );
		if (ImGui::Button(ICON_FA_STOP, PlayModeButtonSize))
		{
			PlayModeController::SetPlayMode(PlayMode::Stopped);
		}

		ImGui::SameLine((ImGui::GetWindowWidth() - 25) * 0.5f + (PlayModeButtonsPadding * 2));
		if (ImGui::Button(ICON_FA_FORWARD_STEP, PlayModeButtonSize))
		{
			PlayModeController::Step();
		}
	}

	void ToolBar::DrawStoppedMode()
	{
		ImGui::SameLine((ImGui::GetWindowWidth() - 25) * 0.5f);
		if (ImGui::Button(ICON_FA_PLAY, PlayModeButtonSize))
		{
			PlayModeController::SetPlayMode(PlayMode::Playing);
		}
	}
}