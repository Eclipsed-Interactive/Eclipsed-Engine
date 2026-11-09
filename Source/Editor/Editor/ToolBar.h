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

		static void CenterButtons(int buttonCount);

	public:
		static inline ImVec2 PlayModeButtonSize = ImVec2(25, 25);

		static inline float PlayModeButtonSpacing = 5.f;
		static inline float ToolBarHeight = 30.f;
	};
}