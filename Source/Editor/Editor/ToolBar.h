#pragma once

#include "Views/EditorViewRegistry.h"

#include "ImGui/imgui_internal.h"
#include <vector>

namespace Eclipse::Editor
{
	class ToolBar
	{
	public:
		static void Draw();

		static void DrawPlayMode();
		static void DrawPausedMode();
		static void DrawStoppedMode();

	public:
		static inline ImVec2 PlayModeButtonSize = ImVec2(25, 25);

		static inline float PlayModeButtonsPadding = 15;
		static inline float ToolBarHeight = 30.f;
	};
}