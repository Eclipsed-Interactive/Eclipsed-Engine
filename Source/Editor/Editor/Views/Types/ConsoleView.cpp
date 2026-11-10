#include "ConsoleView.h"

#include <Core/Logger/DebugLogger.h>

#include <FontAwesome/7/IconsFontAwesome7.h>

#include <vector>
#include <windows.h>

#include <Core/EventSystem/EventSystem.h>

namespace Eclipse::Editor
{
	void ConsoleView::OnOpen()
	{
		// Load settings.

		filter = {};

		if (ClearOnPlay) EventSystem::Subscribe("OnEnterPlaymode", Core::DebugLogger::ClearGameBuffer);
		else EventSystem::Unsubscribe("OnEnterPlaymode", Core::DebugLogger::ClearGameBuffer);
	}

	void ConsoleView::Draw()
	{
		if ((selectedIndex != -1 || selectedMessage) && Core::DebugLogger::GetGameBuffer().GetUniqueMessages().size() == 0)
			ResetSelected();

		// Reset selection if pressing middle mouse button
		if (ImGui::GetIO().MouseClicked[2])
			ResetSelected();

		if ((selectedIndex != -1 || selectedMessage) && ImGui::GetIO().MouseClicked[1])
		{
			ImGui::OpenPopup("##Console Context");
		}

		{ // Drawing toolbar.
			DrawClearButton();

			ImGui::SameLine();

			DrawShouldCollapseButton();

			ImGui::SameLine();
			ImGui::Dummy({ 20, 0 });
			ImGui::SameLine();

			DrawSourceSwitch();

			ImGui::SameLine();
			ImGui::Dummy({ 20, 0 });
			ImGui::SameLine();

			DrawSearchBar();

			ImGui::SameLine();
			ImGui::Dummy({ 20, 0 });
			ImGui::SameLine();

			DrawLogToggles();

			ImGui::SameLine();

			DrawClearOnPlayToggle();
		}

		if (filter.IsActive())
			ResetSelected();

		{ // Message boxes
			DrawMessageBox();
		}

		DrawContextMenu();
	}

	void ConsoleView::DrawSourceToggle(const char* icon, const char* tooltip, bool& enabled)
	{
		const bool wasActive = enabled;
		if (wasActive)
		{
			ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive));
		}

		if (ImGui::Button(icon))
		{
			enabled = !enabled;
			ResetSelected();
		}

		if (ImGui::IsItemHovered())
		{
			ImGui::SetTooltip(tooltip);
		}

		if (wasActive)
		{
			ImGui::PopStyleColor(2);
		}

	}

	void ConsoleView::ResetSelected()
	{
		selectedIndex = -1;
		selectedMessage = nullptr;
	}



	void ConsoleView::DrawLogMessages(const std::vector<ConsoleRow>& messages)
	{
		if (IsCollapsing) DrawCollapsed(messages);
		else DrawInstances(messages);
	}

	void ConsoleView::DrawSourceSwitch()
	{
		auto button = [this](const char* icon, const char* tooltip, bool value) {
			const bool active = DrawEngineLogs == value;
			ImGui::PushStyleColor(ImGuiCol_Button, active ? ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive) : ImVec4(0.15f, 0.15f, 0.15f, 1.f));
			if (ImGui::Button(icon) && !active)
			{
				DrawEngineLogs = value;
				selectedIndex = -1;
			}
			ImGui::PopStyleColor();

			if (ImGui::IsItemHovered()) ImGui::SetTooltip(tooltip);
			};

		button(ICON_FA_MICROCHIP, "Engine logs", true);
		ImGui::SameLine(0.f, 0.f);
		button(ICON_FA_GAMEPAD, "Game logs", false);
	}

	void ConsoleView::DrawClearOnPlayToggle()
	{
		const char* text = "Clear on play";
		const char* tooltip = "Clears the console when entering playmode.";

		const float width = ImGui::CalcTextSize(text).x + ImGui::GetStyle().ItemSpacing.x + ImGui::GetFrameHeight();
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x - width);

		ImGui::TextUnformatted(text);
		ImGui::SameLine();
		if (ImGui::Checkbox("##clear-on-play", &ClearOnPlay))
		{
			if (ClearOnPlay) EventSystem::Subscribe("OnEnterPlaymode", Core::DebugLogger::ClearGameBuffer);
			else EventSystem::Unsubscribe("OnEnterPlaymode", Core::DebugLogger::ClearGameBuffer);
		}

		if (ImGui::IsItemHovered()) ImGui::SetTooltip(tooltip);
	}

	void ConsoleView::DrawMessageBox()
	{
		const Core::DebugLoggerBuffer& buffer = DrawEngineLogs ? Core::DebugLogger::GetEngineBuffer() : Core::DebugLogger::GetGameBuffer();

		std::vector<ConsoleRow> messages;
		GetMessages(buffer, messages);

		ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.35f, 0.35f, 0.40f, 1.0f));
		ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 1.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 4.0f);


		const float contentSize = (selectedIndex != -1 || selectedMessage) ? ImGui::GetContentRegionAvail().y * 0.4f : 0;

		ImGui::BeginChild("##console-view", ImVec2(0, contentSize), ImGuiChildFlags_Borders);

		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor();

		ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(6.0f, 0.0f));
		ImGui::PushStyleColor(ImGuiCol_TableBorderLight, ImVec4(1.0f, 1.0f, 1.0f, 0.06f));

		const int tableCount = IsCollapsing ? 3 : 2;

		if (ImGui::BeginTable("##console-table", tableCount, ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_BordersInnerH))
		{
			ImGui::TableSetupColumn("##icon", ImGuiTableColumnFlags_WidthFixed, Row_Height * 1.25f);
			ImGui::TableSetupColumn("##message", ImGuiTableColumnFlags_WidthStretch);

			DrawLogMessages(messages);

			ImGui::EndTable();

			ImGui::PopStyleColor();
			ImGui::PopStyleVar();
		}

		{
			const bool stickToBottom = ImGui::GetScrollY() >= ImGui::GetScrollMaxY() - 1.0f;
			if (stickToBottom)
				ImGui::SetScrollHereY(1.f);

			messageBoxWasAtBottom = stickToBottom;
		}

		ImGui::EndChild();

		DrawSelectedMessageBox(messages);
	}

	void ConsoleView::DrawSelectedMessageBox(const std::vector<ConsoleRow>& messages)
	{
		if (selectedIndex == -1 && !selectedMessage) return;

		const Core::DebugLoggerBuffer& buffer = DrawEngineLogs ? Core::DebugLogger::GetEngineBuffer() : Core::DebugLogger::GetGameBuffer();


		ImGui::BeginChild("##selected_message_box");
		if (selectedIndex != -1 && buffer.GetMessageInstances().Count() > 0)
		{
			const ConsoleRow& row = messages[selectedIndex];

			const DebugMessageInstance& message = buffer.GetMessageInstances().Get(row.instanceIndex);
			SeverityStyle style = GetSeverityStyle(message.message->diagnosticSeverity);

			const float footerHeight = ImGui::GetStyle().ItemSpacing.y * 2.0f + ImGui::GetTextLineHeightWithSpacing();

			ImGui::SetCursorPosX(10);
			DrawSeverityIconCenteredY(style, ImGui::GetContentRegionAvail().y - footerHeight);

			ImGui::SameLine();
			ImGui::BeginChild("##message_box_selection", ImVec2(0, -footerHeight));
			ImGui::TextWrapped(message.message->message.c_str());
			ImGui::EndChild();

			ImGui::Separator();
			ImGui::BeginDisabled();
			ImGui::Text(std::format("{:%T}", message.timeStamp).c_str());
			ImGui::SameLine();
			ImGui::Text(" | ");
			ImGui::SameLine();
			ImGui::Text("Line");
			ImGui::SameLine();
			ImGui::Text(std::to_string(message.line).c_str());
			ImGui::SameLine();
			ImGui::Text(" | ");
			ImGui::SameLine();
			ImGui::Text(message.message->file.c_str());
			ImGui::EndDisabled();
		}

		else if (selectedMessage)
		{
			SeverityStyle style = GetSeverityStyle(selectedMessage->diagnosticSeverity);

			const float footerHeight = ImGui::GetStyle().ItemSpacing.y * 2.0f + ImGui::GetTextLineHeightWithSpacing();

			ImGui::SetCursorPosX(10);
			DrawSeverityIconCenteredY(style, ImGui::GetContentRegionAvail().y - footerHeight);

			ImGui::SameLine();
			ImGui::BeginChild("##message_box_selection", ImVec2(0, -footerHeight));
			ImGui::TextWrapped(selectedMessage->message.c_str());
			ImGui::EndChild();

			ImGui::Separator();
			ImGui::BeginDisabled();
			ImGui::Text("Count");
			ImGui::SameLine();
			ImGui::Text(std::to_string(selectedMessage->count).c_str());
			ImGui::SameLine();
			ImGui::Text(" | ");
			ImGui::SameLine();
			ImGui::Text(selectedMessage->file.c_str());
			ImGui::EndDisabled();
		}

		ImGui::EndChild();
	}

	void ConsoleView::DrawContextMenu()
	{
		if (ImGui::BeginPopup("##Console Context"))
		{
			if (ImGui::Button("Copy"))
			{
				const DebugMessage* messagePtr = selectedMessage;

				if (selectedIndex != -1)
				{
					const Core::DebugLoggerBuffer& buffer = DrawEngineLogs ? Core::DebugLogger::GetEngineBuffer() : Core::DebugLogger::GetGameBuffer();
					messagePtr = buffer.GetMessageInstances().Get(selectedIndex).message;
				}

				const char* message = messagePtr->message.c_str();
				const size_t lenght = messagePtr->message.size() + 1;

				HGLOBAL hgl = GlobalAlloc(GMEM_MOVEABLE, lenght);
				memcpy(GlobalLock(hgl), message, lenght);
				GlobalUnlock(hgl);

				OpenClipboard(NULL);
				EmptyClipboard();
				SetClipboardData(CF_TEXT, hgl);
				CloseClipboard();

				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}

	bool ConsoleView::PassSearchFilter(const DebugMessage* message)
	{
		const char* messageStr = message->message.c_str();
		const char* messageFileStr = message->file.c_str();

		bool passed = filter.PassFilter(messageStr);
		passed |= filter.PassFilter(messageFileStr);

		return passed;
	}

	bool ConsoleView::PassFilter(const DebugMessage* message)
	{
		bool passed = PassSearchFilter(message);
		
		switch (message->diagnosticSeverity)
		{
		case DiagnosticSeverity::Info: passed &= DrawInfoLogs; break;
		case DiagnosticSeverity::Warning: passed &= DrawWarningLogs; break;
		case DiagnosticSeverity::Error: passed &= DrawErrorLogs; break;
		}

		return passed;
	}

	void ConsoleView::DrawSearchBar()
	{
		filter.Draw("##console-search-bar", ImGui::GetContentRegionAvail().x - 400);

		ImGui::SameLine(0.f, 0.f);
		if (ImGui::Button(ICON_FA_XMARK))
		{
			filter.Clear();
		}
	}

	void ConsoleView::DrawLogToggles()
	{
		DrawLogToggle(DiagnosticSeverity::Error, "Error", DrawErrorLogs);
		ImGui::SameLine();
		DrawLogToggle(DiagnosticSeverity::Warning, "Warning", DrawWarningLogs);
		ImGui::SameLine();
		DrawLogToggle(DiagnosticSeverity::Info, "Info", DrawInfoLogs);
	}

	void ConsoleView::DrawLogToggle(DiagnosticSeverity severity, const char* label, bool& toggle)
	{
		SeverityStyle style = GetSeverityStyle(severity);
		const bool wasActive = toggle;

		const ImGuiStyle& s = ImGui::GetStyle();
		const float spacing = s.ItemInnerSpacing.x;

		const ImVec2 size = ImVec2(
			s.FramePadding.x * 2 + ImGui::CalcTextSize(style.icon).x + spacing + ImGui::CalcTextSize(label).x,
			ImGui::GetFrameHeight()
		);

		ImGui::PushID(label);
		ImGui::BeginGroup();

		if (wasActive)
		{
			ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive));
		}

		const ImVec2 start = ImGui::GetCursorPos();
		if (ImGui::Button("##logtoggle", size))
			toggle = !toggle;

		if (wasActive)
			ImGui::PopStyleColor(2);


		ImGui::SetCursorPos(ImVec2(start.x + s.FramePadding.x, start.y + s.FramePadding.y));
		ImGui::TextColored(style.color, style.icon);
		ImGui::SameLine(0, spacing);
		ImGui::TextUnformatted(label);

		ImGui::EndGroup();
		ImGui::PopID();
	}

	void ConsoleView::DrawCollapsed(const std::vector<ConsoleRow>& messages)
	{
		ImGui::TableSetupColumn("##count", ImGuiTableColumnFlags_WidthFixed, 40.0f);

		ImGuiListClipper clipper;
		clipper.Begin(static_cast<int>(messages.size()), Row_Height);
		while (clipper.Step())
		{
			for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
			{
				const ConsoleRow& message = messages[i];
				const SeverityStyle style = GetSeverityStyle(message.message->diagnosticSeverity);

				ImGui::PushID(i);
				ImGui::TableNextRow(ImGuiTableRowFlags_None, Row_Height);

				ImGui::TableSetColumnIndex(0);
				const ImVec2 cellStart = ImGui::GetCursorPos();

				ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(ImGui::GetStyle().ItemSpacing.x, 0.0f));

				const bool clicked = ImGui::Selectable("##row", selectedMessage == message.message,
					ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowOverlap,
					ImVec2(0.f, Row_Height));
				ImGui::PopStyleVar();

				if (clicked) selectedMessage = message.message;

				DrawSeverityIcon(style, cellStart, Row_Height);

				ImGui::TableSetColumnIndex(1);

				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 4);
				ImGui::TextUnformatted(message.message->message.c_str());

				const std::string countText = std::to_string(message.message->count);
				const float textWidth = ImGui::CalcTextSize(countText.c_str()).x;

				ImGui::TableSetColumnIndex(2);
				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x - textWidth);
				ImGui::TextUnformatted(countText.c_str());

				ImGui::PopID();
			}
		}
	}

	void ConsoleView::DrawInstances(const std::vector<ConsoleRow>& messages)
	{
		const Core::DebugLoggerBuffer& buffer = DrawEngineLogs ? Core::DebugLogger::GetEngineBuffer() : Core::DebugLogger::GetGameBuffer();

		ImGuiListClipper clipper;
		clipper.Begin(static_cast<int>(messages.size()), Row_Height);
		while (clipper.Step())
		{
			int index = 0;
			for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
			{
				const bool wasSelected = i == selectedIndex;
				const ConsoleRow& row = messages[i];
				const DebugMessage* message = row.message;

				const SeverityStyle style = GetSeverityStyle(message->diagnosticSeverity);

				ImGui::PushID(i);
				ImGui::TableNextRow(ImGuiTableRowFlags_None, Row_Height);

				ImGui::TableSetColumnIndex(0);
				const ImVec2 cellStart = ImGui::GetCursorPos();

				ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(ImGui::GetStyle().ItemSpacing.x, 0.0f));
				const bool clicked = ImGui::Selectable("##row", selectedIndex == i,
					ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowOverlap,
					ImVec2(0.f, Row_Height));
				ImGui::PopStyleVar();

				if (clicked) selectedIndex = i;

				DrawSeverityIcon(style, cellStart, Row_Height);

				ImGui::TableSetColumnIndex(1);

				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 4);
				ImGui::TextUnformatted("[");
				ImGui::SameLine(0, 0);

				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.42f, 0.42f, 0.45f, 1.0f));
				ImGui::TextUnformatted(std::format("{:%T}", buffer.GetMessageInstances().Get(row.instanceIndex).timeStamp).c_str());
				ImGui::PopStyleColor();
				ImGui::SameLine(0, 0);

				ImGui::TextUnformatted("]");
				ImGui::SameLine();

				ImGui::TextUnformatted(message->message.c_str());

				ImGui::PopID();
			}
		}
	}

	void ConsoleView::DrawShouldCollapseButton()
	{
		const bool wasActive = IsCollapsing;
		if (wasActive)
		{
			ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive));
		}

		if (ImGui::Button(ICON_FA_LAYER_GROUP))
		{
			IsCollapsing = !IsCollapsing;
			ResetSelected();
		}

		if (ImGui::IsItemHovered())
			ImGui::SetTooltip("Collapse.");

		if (wasActive)
			ImGui::PopStyleColor(2);
	}

	void ConsoleView::DrawClearButton()
	{
		if (ImGui::Button(ICON_FA_TRASH_CAN))
		{
			Core::DebugLogger::ClearBuffers();
		}

		if (ImGui::IsItemHovered())
			ImGui::SetTooltip("Clear.");
	}

	void ConsoleView::DrawSeverityIcon(const SeverityStyle& style, ImVec2 cellStart, float rowHeight)
	{
		const float cellWidth = ImGui::GetContentRegionAvail().x;

		ImGui::PushFont(nullptr, rowHeight * 0.8f);

		const ImVec2 iconSize = ImGui::CalcTextSize(style.icon);
		ImGui::SetCursorPos(ImVec2(
			cellStart.x + (cellWidth - iconSize.x) * 0.5f,
			cellStart.y + (rowHeight - iconSize.y) * 0.5f));
		ImGui::TextColored(style.color, style.icon);
		ImGui::PopFont();
	}

	void ConsoleView::DrawSeverityIconCenteredY(const SeverityStyle& style, float rowHeight)
	{
		if (rowHeight < 0) return;

		ImGui::PushFont(nullptr, rowHeight * 0.8f);

		const ImVec2 iconSize = ImGui::CalcTextSize(style.icon);
		ImGui::SetCursorPosY((rowHeight - iconSize.y) * 0.5f);

		ImGui::TextColored(style.color, style.icon);
		ImGui::PopFont();
	}

	SeverityStyle ConsoleView::GetSeverityStyle(DiagnosticSeverity severity)
	{
		switch (severity)
		{
		case Eclipse::DiagnosticSeverity::Warning: return { ICON_FA_CIRCLE_XMARK, ImVec4(0.95f, 0.76f, 0.31f, 1.f) };
		case Eclipse::DiagnosticSeverity::Error: return { ICON_FA_TRIANGLE_EXCLAMATION, ImVec4(0.94f, 0.38f, 0.36f, 1.f) };
		case Eclipse::DiagnosticSeverity::Info:
		default: return { ICON_FA_CIRCLE_INFO, ImVec4(0.42f, 0.71f, 0.93f, 1.f) };
		}
	}


















	void ConsoleView::GetMessages(const Core::DebugLoggerBuffer& buffer, std::vector<ConsoleRow>& outMessages)
	{
		if (IsCollapsing)
		{
			for (const DebugMessage& message : buffer.GetUniqueMessages())
			{
				if (!PassFilter(&message)) continue;

				outMessages.push_back(ConsoleRow{ .message = &message });
			}
		}
		else
		{
			for (int i = 0; i < buffer.GetMessageInstances().Count(); i++)
			{
				const DebugMessageInstance& instance = buffer.GetMessageInstances().Get(i);

				if (!PassFilter(instance.message)) continue;

				outMessages.push_back(ConsoleRow{ .message = instance.message, .instanceIndex = i });
			}

		}
	}
}