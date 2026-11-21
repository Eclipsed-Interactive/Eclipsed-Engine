#include "DevConsole.h"
#include "ImGui/imgui.h"

#include "Core/Timer.h"

#include <algorithm>
#include <sstream>
#include <cctype>


namespace Eclipse
{
	std::unordered_map<std::string, DevConsole::Command> DevConsole::Commands;
	std::vector<std::string> DevConsole::RecentCommands;
	bool DevConsole::ConsoleOpen = false;
	int DevConsole::MaxRecentCommands = 20;
	int DevConsole::CurrentHistoryIndex = 0;
	bool DevConsole::ImguiDemoWindow = false;

	std::string ToLower(std::string s)
	{
		std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return (char)std::tolower(c); });
		return s;
	}

	void DevConsole::Update()
	{
		if (ImguiDemoWindow)
			ImGui::ShowDemoWindow();

		if (ImGui::IsKeyPressed(ImGuiKey_F1, false))
		{
			ConsoleOpen = !ConsoleOpen;
		}
	}

	void DevConsole::Init()
	{
		RegisterCommand("show imguidemo", []() {
				ImguiDemoWindow = true;
			});

	}

	void DevConsole::Draw()
	{
		Update();

		if (!ConsoleOpen)
			return;

		const ImGuiViewport* viewport = ImGui::GetMainViewport();

		float Height = ImGui::GetFrameHeight();

		ImGui::SetNextWindowViewport(viewport->ID);

		ImVec2 ConsoleSize = ImVec2(viewport->WorkSize.x, Height);
		ImGui::SetNextWindowSize(ConsoleSize);

		float ExtraHeight = Height + ImGui::GetStyle().WindowPadding.y;
		ImVec2 ConsolePosition = ImVec2(viewport->WorkPos.x, viewport->WorkPos.y + viewport->WorkSize.y - ExtraHeight);
		ImGui::SetNextWindowPos(ConsolePosition, ImGuiCond_Always);

		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.f);

		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.f, 0.f, 0.f, 0.85f));
		ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0, 0, 0, 0));

		ImGui::Begin("##DevConsole", nullptr, ImGuiWindowFlags_NoDecoration |
			ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings |
			ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoScrollbar |
			ImGuiWindowFlags_NoScrollWithMouse);

		ImGui::SetKeyboardFocusHere();

		ImGui::SetCursorPosY(6);
		ImGui::TextUnformatted(">");

		ImGui::SameLine();

		ImGui::SetNextItemWidth(-FLT_MIN);

		ImGui::SetCursorPosY(4);
		char CommandBuffer[512];
		memset(CommandBuffer, 0, 511);
		memset(CommandBuffer + 511, (int)"\0", 1);
		bool EnteredCommand = ImGui::InputText("##CommandConsole", CommandBuffer, 512, 
			ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackHistory |
			ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_CallbackCharFilter,
			[](ImGuiInputTextCallbackData* d) { return DevConsole::InputCallback(d); });

		if (EnteredCommand)
			ExecuteCommand(CommandBuffer);

		ImGui::End();

		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor(4);
	}

	void DevConsole::ExecuteCommand(const char* Command)
	{
		std::string CommandString = std::string(Command);

		CommandString = ToLower(CommandString);

		if (RecentCommands.empty() || RecentCommands.back() != Command)
			RecentCommands.push_back(Command);
		CurrentHistoryIndex = -1;

		auto CommandIterator = Commands.find(CommandString);
		if (CommandIterator == Commands.end())
			return;

		CommandIterator->second.Function();
	}

	void DevConsole::RegisterCommand(const char* Command, std::function<void()> Function)
	{
		std::string CommandString = std::string(Command);

		std::transform(CommandString.begin(), CommandString.end(), CommandString.begin(),
			[](unsigned char c) { return std::tolower(c); }
		);

		Commands.emplace(CommandString, Function);
	}
	int DevConsole::InputCallback(void* rawData)
	{
		ImGuiInputTextCallbackData* data = (ImGuiInputTextCallbackData*)rawData;

		if (data->EventFlag == ImGuiInputTextFlags_CallbackHistory && !RecentCommands.empty())
		{
			const int prev = CurrentHistoryIndex;
			if (data->EventKey == ImGuiKey_UpArrow)
			{
				if (CurrentHistoryIndex == -1)
					CurrentHistoryIndex = (int)RecentCommands.size() - 1;
				else if (CurrentHistoryIndex > 0)
					CurrentHistoryIndex--;
			}
			else if (data->EventKey == ImGuiKey_DownArrow)
			{
				if (CurrentHistoryIndex != -1 && ++CurrentHistoryIndex >= (int)RecentCommands.size())
					CurrentHistoryIndex = -1;
			}

			if (prev != CurrentHistoryIndex)
			{
				data->DeleteChars(0, data->BufTextLen);
				if (CurrentHistoryIndex >= 0)
					data->InsertChars(0, RecentCommands[CurrentHistoryIndex].c_str());
			}
		}
		else if (data->EventFlag == ImGuiInputTextFlags_CallbackCompletion)
		{
			std::string typed = ToLower(std::string(data->Buf, data->BufTextLen));
			for (auto& [name, cmd] : Commands)
			{
				if (name.rfind(typed, 0) == 0)
				{
					data->DeleteChars(0, data->BufTextLen);
					data->InsertChars(0, name.c_str());
					break;
				}
			}
		}
		else if (data->EventFlag == ImGuiInputTextFlags_CallbackCharFilter)
		{
			if (data->EventChar == '`')
				return 1;
		}

		return 0;
	}
}
