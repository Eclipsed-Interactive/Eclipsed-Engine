#pragma once

#include "Editor/Views/IEditorView.h"
#include <Core/Logger/DebugMessage.h>
#include <Core/Logger/DebugLoggerBuffer.h>
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
		void DrawSeverityIconCenteredY(const SeverityStyle& style, float rowHeight);


		static SeverityStyle GetSeverityStyle(DiagnosticSeverity severity);

		void DrawSourceToggle(const char* icon, const char* tooltip, bool& enabled);

		void ResetSelected();

	private:
		void DrawCollapsed(const Core::DebugLoggerBuffer& logBuffer);
		void DrawInstances(const Core::DebugLoggerBuffer& logBuffer);

		void DrawLogMessages(const Core::DebugLoggerBuffer& logBuffer);

		void DrawSourceSwitch();

		void DrawClearOnPlayToggle();
		void DrawMessageBox();
		void DrawSelectedMessageBox();

		void DrawContextMenu();

		bool PassSearchFilter(const DebugMessage* message);
		void DrawSearchBar();

	private:
		void DrawSelectedMessage(const Core::DebugLoggerBuffer& buffer, const DebugMessage* message);

	private:
		void DrawLogToggles();

		void DrawLogToggle(DiagnosticSeverity severity, const char* label, bool& toggle);

	private:
		ImGuiTextFilter filter;

		bool messageBoxWasAtBottom = true;

		constexpr static float Row_Height = 40;
		int selectedIndex = -1;
		const DebugMessage* selectedMessage = nullptr;

		bool IsCollapsing = false;

		bool ClearOnPlay = true;

		bool DrawInfoLogs = true;
		bool DrawWarningLogs = true;
		bool DrawErrorLogs = true;

		bool DrawEngineLogs = false;
	};
}