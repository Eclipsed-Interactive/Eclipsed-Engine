#include "ConsoleView.h"

#include <Core/Logger/DebugLogger.h>

#include <FontAwesome/7/IconsFontAwesome7.h>

namespace Eclipse::Editor
{
	void ConsoleView::OnOpen()
	{

	}

	void ConsoleView::Draw()
	{
		DrawClearButton();

		ImGui::SameLine();

		DrawShouldCollapseButton();

		ImGui::SameLine();

		ImGui::DragFloat("##aawdwadwad", &size, 0.01f);


		ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.35f, 0.35f, 0.40f, 1.0f)); // outline color
		ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 1.0f);                  // thickness
		ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 4.0f);                    // rounded corners

		ImGui::BeginChild("##console-view", ImVec2(0, 0), ImGuiChildFlags_Borders);

		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor();

		const auto& messageBuffer = Core::DebugLogger::GetMessages();

		const float rowHeight = size; // tune this only

		ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(6.0f, 0.0f));
		ImGui::PushStyleColor(ImGuiCol_TableBorderLight, ImVec4(1.0f, 1.0f, 1.0f, 0.06f));

		if (ImGui::BeginTable("##console-table", 2, ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_BordersInnerH))
		{
			ImGui::TableSetupColumn("##icon", ImGuiTableColumnFlags_WidthFixed, rowHeight * 1.25f);
			//ImGui::TableSetupColumn("##time", ImGuiTableColumnFlags_WidthFixed);
			ImGui::TableSetupColumn("##message", ImGuiTableColumnFlags_WidthStretch);

			const float textOffsetY = (rowHeight - ImGui::GetTextLineHeight()) * 0.5f;

			ImGuiListClipper clipper;
			clipper.Begin(static_cast<int>(messageBuffer.Count()), rowHeight);
			while (clipper.Step())
			{
				for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
				{
					const bool wasSelected = i == selectedIndex;

					const auto& mes = messageBuffer.Get(i);
					const SeverityStyle style = GetSeverityStyle(mes.message->diagnosticSeverity);

					ImGui::PushID(i);
					ImGui::TableNextRow(ImGuiTableRowFlags_None, rowHeight);

					ImGui::TableSetColumnIndex(0);
					const ImVec2 cellStart = ImGui::GetCursorPos();

					if (ImGui::Selectable("##row", selectedIndex == i, 
						ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowOverlap,
						ImVec2(0.f, rowHeight)))
					{
						selectedIndex = i;
					}

					ImGui::SetCursorPosY(ImGui::GetCursorPosY() + textOffsetY); 
					DrawSeverityIcon(style, cellStart, rowHeight);

					ImGui::TableSetColumnIndex(1);
					ImGui::SetCursorPosY(ImGui::GetCursorPosY() + textOffsetY);
					ImGui::TextUnformatted(mes.message->message.c_str());

					ImGui::PopID();
				}
			}

			ImGui::EndTable();
		}

		ImGui::PopStyleColor();
		ImGui::PopStyleVar();

		if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
			ImGui::SetScrollHereY(1.f);

		ImGui::EndChild();
	}

	void ConsoleView::DrawShouldCollapseButton()
	{
		const bool wasActive = isCollapsing;
		if (wasActive)
		{
			ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive));
		}

		if (ImGui::Button(ICON_FA_LAYER_GROUP))
		{
			isCollapsing = !isCollapsing;
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
			Core::DebugLogger::Clear();
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
}