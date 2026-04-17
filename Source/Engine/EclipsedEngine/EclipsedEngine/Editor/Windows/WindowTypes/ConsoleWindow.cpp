#ifdef ECLIPSED_EDITOR
#include "ConsoleWindow.h"

#include <ImGui/imgui.h>
#include "CoreEngine/Debug/DebugLogger.h"

namespace Eclipse::Editor
{
	void ConsoleWindow::Update()
	{
		ImGui::Checkbox("Show Messages", &myShouldShowMessages);
		ImGui::SameLine();
		ImGui::Checkbox("Show Warnings", &myShouldShowWarnings);
		ImGui::SameLine();
		ImGui::Checkbox("Show Errors", &myShouldShowErrors);
		ImGui::SameLine();
		ImGui::Checkbox("Collapse", &myShouldCollapseMessages);
		ImGui::SameLine();

		if (ImGui::Button("Clear"))
			DebugLogger::Clear();

		ImGui::Separator();

		ImGui::BeginChild(
			"ConsoleScrollRegion",
			ImVec2(0, 0),
			false,
			ImGuiWindowFlags_AlwaysVerticalScrollbar
		);

		const auto& messageBuffer = DebugLogger::GetMessages();

		std::vector<const DebugMessage*> filteredMessages;
		filteredMessages.reserve(messageBuffer.Count());

		for (int i = 0; i < messageBuffer.Count(); i++)
		{
			const DebugMessage& msg = messageBuffer.Get(i);

			if ((msg.diagnosticSeverity == DiagnosticSeverity::Info && !myShouldShowMessages) ||
				(msg.diagnosticSeverity == DiagnosticSeverity::Warning && !myShouldShowWarnings) ||
				(msg.diagnosticSeverity == DiagnosticSeverity::Error && !myShouldShowErrors))
				continue;

			filteredMessages.push_back(&msg);
		}

		std::unordered_map<std::string, int> collapseMap;
		int count = 0;

		if (myShouldCollapseMessages)
		{
			for (int i = 0; i < filteredMessages.size(); i++)
			{
				const DebugMessage& msg = *filteredMessages[i];

				collapseMap[msg.message]++;

				if (collapseMap[msg.message] > 1)
					continue;
			}

			count = collapseMap.size();
		}
		else count = filteredMessages.size();

		ImGuiListClipper clipper;
		clipper.Begin(count);
		while (clipper.Step())
		{
			for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
			{
				const DebugMessage& msg = *filteredMessages[i];

				if (myShouldCollapseMessages)
				{
					DisplayMessage(msg);

					ImGui::SameLine();
					ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "%d", collapseMap[msg.message]);
					ImGui::Separator();
					continue;
				}

				DisplayMessage(msg);
				ImGui::Separator();
			}
		}

		ImGui::EndChild();
	}


	void ConsoleWindow::DisplayMessage(const DebugMessage& aMessage)
	{
		ImVec4 color;
		switch (aMessage.diagnosticSeverity)
		{
		case DiagnosticSeverity::Info: color = { 1.f,1.f,1.f,1.f }; break;
		case DiagnosticSeverity::Warning: color = { 1.f,1.f,0.f,1.f }; break;
		case DiagnosticSeverity::Error:	color = { 1.f,0.f,0.f,1.f }; break;
		}

		static ImVec4 fileColor = { 0.7f, 0.7f, 0.7f, 1.f };
		static ImVec4 lineColor = { 0.88f, 0.88f, 0.2f, 1.f };
		static ImVec4 timeColor = { 0.5f, 0.5f, 0.5f, 1.f };

		ImGui::TextColored(color, aMessage.message.c_str());

		ImGui::Spacing();
		ImGui::SetWindowFontScale(0.9f);

		switch (aMessage.diagnosticKind)
		{
		case DiagnosticKind::Text: {
			break;
		};

		case DiagnosticKind::SourceCode: {
			ImGui::TextColored(fileColor, aMessage.file.c_str());
			ImGui::TextColored(fileColor, "At line: ");
			ImGui::SameLine();
			ImGui::TextColored(lineColor, std::to_string(aMessage.line).c_str());
			ImGui::TextColored(timeColor, aMessage.timeStamp.c_str());
			ImGui::SetWindowFontScale(1.f);
			break;
		};
		}
	}
}

#endif