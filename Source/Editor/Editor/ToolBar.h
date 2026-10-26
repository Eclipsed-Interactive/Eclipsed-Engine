#pragma once

#include "Views/EditorViewRegistry.h"

#include "ImGui/imgui_internal.h"
#include <vector>

namespace Eclipse::Editor
{
	enum class PlayMode
	{
		Playing,
		Paused,
		Stopped
	};

	class ToolBar
	{
	public:
		static void Draw();

		static void DrawPlayMode(PlayMode& currentMode);
		static void DrawPausedMode(PlayMode& currentMode);
		static void DrawStoppedMode(PlayMode& currentMode);

	public:
		static inline ImVec2 PlayModeButtonSize = ImVec2(25, 25);

		static inline float PlayModeButtonsPadding = 15;
		static inline float ToolBarHeight = 30.f;
	};
}