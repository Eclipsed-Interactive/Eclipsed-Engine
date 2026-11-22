#include "DevConsole.h"
#include "ImGui/imgui.h"

#include "Core/Timer.h"

#include <algorithm>
#include <sstream>
#include <cctype>

#include "EclipsedEngine/Components/Rendering/SpriteRenderer2D.h"


namespace Eclipse
{
	std::unordered_map<std::string, DevConsole::Command> DevConsole::Commands;
	std::vector<std::string> DevConsole::RecentCommands;
	std::vector<std::string> DevConsole::ConsoleLog;
	int DevConsole::CurrentHistoryIndex = 0;
	bool DevConsole::ImguiDemoWindow = false;
	int DevConsole::CurrentOpenState = 0;
	bool DevConsole::SetSrollToBottom = false;
	int DevConsole::CommandlogHeight = 400;

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
			CurrentOpenState = (CurrentOpenState + 1) % 3;
		}
	}

	void DevConsole::Init()
	{
		RegisterCommand("show imguidemo", []() {
				ImguiDemoWindow = !ImguiDemoWindow;
			});
		 
		RegisterCommand("help", []() {
			AddToConsoleLog("Commands that are available are (show imguidemo)");
			});

		RegisterCommand("freezerendering", []() {
			SpriteRenderer2D::RenderingFrozen = !SpriteRenderer2D::RenderingFrozen;
			});

	}

	void DevConsole::Draw()
	{
		Update();

		if (CurrentOpenState == 0)
			return;

		const ImGuiViewport* Viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowViewport(Viewport->ID);

		float Height = ImGui::GetFrameHeight();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.f);

		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.f, 0.f, 0.f, 0.9f));
		ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0, 0, 0, 0));

		if (CurrentOpenState == 1)
		{
			float ExtraHeight = Height + ImGui::GetStyle().WindowPadding.y;

			float HeightPosition = Viewport->WorkPos.y + Viewport->WorkSize.y - ExtraHeight;
			DrawCommandLine(Viewport, HeightPosition);
		}
		else if(CurrentOpenState == 2)
		{
			DrawCommandLog(Viewport, Viewport->Pos.y);
			DrawCommandLine(Viewport, Viewport->Pos.y + CommandlogHeight);
		}

		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor(4);
	}

	void DevConsole::DrawCommandLog(const ImGuiViewport* Viewport, float HeightPosition)
	{
		float Height = ImGui::GetFrameHeight();

		ImVec2 ConsoleSize = ImVec2(Viewport->WorkSize.x, CommandlogHeight);
		ImGui::SetNextWindowSize(ConsoleSize);

		ImVec2 ConsolePosition = ImVec2(Viewport->WorkPos.x, HeightPosition);
		ImGui::SetNextWindowPos(ConsolePosition, ImGuiCond_Always);

		int ImGuiFlags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings |
			ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse;

		if (ConsoleLog.empty())
			ImGuiFlags |= ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;

		ImGui::Begin("##DevConsoleLog", nullptr, ImGuiFlags);

		if (SetSrollToBottom)
		{
			SetSrollToBottom = false;
			ImGui::SetScrollY(ImGui::GetScrollMaxY() + 16);
		}

		std::stringstream LogStream;
		for (auto& LogEntry : ConsoleLog)
			LogStream << LogEntry.c_str() << "\n";

		ImVec2 TextSize = ImGui::CalcTextSize("Test");
		float DummySize = ConsoleLog.size() * TextSize.y;

		ImGui::Dummy(ImVec2(0, CommandlogHeight - DummySize - TextSize.y - 8));

		ImGui::TextUnformatted(LogStream.str().c_str());

		ImGui::Separator();

		ImGui::End();
	}

	void DevConsole::DrawCommandLine(const ImGuiViewport* Viewport, float HeightPosition)
	{
		float Height = ImGui::GetFrameHeight();

		ImVec2 ConsoleSize = ImVec2(Viewport->WorkSize.x, Height);
		ImGui::SetNextWindowSize(ConsoleSize);

		ImVec2 ConsolePosition = ImVec2(Viewport->WorkPos.x, HeightPosition);
		ImGui::SetNextWindowPos(ConsolePosition, ImGuiCond_Always);

		ImGui::Begin("##DevConsole", nullptr, ImGuiWindowFlags_NoDecoration |
			ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings |
			ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoScrollWithMouse);

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

		if (EnteredCommand && !std::string(CommandBuffer).empty())
		{
			ExecuteCommand(CommandBuffer);
		}

		ImGui::End();
	}

	void DevConsole::ExecuteCommand(const char* Command)
	{
		SetSrollToBottom = true;

		std::string CommandString = std::string(Command);

		AddToConsoleLog(CommandString.c_str());

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

	void DevConsole::AddToConsoleLog(const char* Log)
	{
		ConsoleLog.emplace_back(Log);
	}
}
