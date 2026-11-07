#pragma once

#include "Editor/Views/IEditorView.h"
#include <Core/Logger/DebugMessage.h>
#include <ImGui/imgui.h>

namespace Eclipse::Editor
{
	struct SeverityStyle
	{
		const char* icon;
		ImVec4 color;
	};

	class ConsoleView : public EditorView<ConsoleView>
	{
		BASIC_VIEW("Console")

	public:
		void OnOpen() override;
		void Draw() override;

	private:
		void DrawShouldCollapseButton();
		void DrawClearButton();

	private:
		void DrawSeverityIcon(const SeverityStyle& style, ImVec2 cellStart, float rowHeight);

		static SeverityStyle GetSeverityStyle(DiagnosticSeverity severity);

	private:
		float size = 24.f;
		int selectedIndex = -1;
		bool isCollapsing = false;
	};
}