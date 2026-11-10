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

	struct ConsoleRow
	{
		const DebugMessage* message = nullptr;
		int instanceIndex = -1;
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

		void GetMessages(const Core::DebugLoggerBuffer& buffer, std::vector<ConsoleRow>& outMessages);

	private:
		void DrawCollapsed(const std::vector<ConsoleRow>& messages);
		void DrawInstances(const std::vector<ConsoleRow>& messages);

		void DrawLogMessages(const std::vector<ConsoleRow>& messages);

		void DrawSourceSwitch();

		void DrawClearOnPlayToggle();
		void DrawMessageBox();
		void DrawSelectedMessageBox(const std::vector<ConsoleRow>& messages);

		void DrawContextMenu();

		bool PassSearchFilter(const DebugMessage* message);
		bool PassFilter(const DebugMessage* message);
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