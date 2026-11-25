#pragma once

#include <functional>
#include <vector>
#include <unordered_map>
#include <queue>

#include <string>

#ifndef ECL_SHIPPING
	#define ECL_DEV_CONSOLE_ENABLED 1
#else
	#define ECL_DEV_CONSOLE_ENABLED 0
#endif

struct ImGuiViewport;

namespace Eclipse
{
	class DevConsole
	{
	public:
		static void Update();
		static void Init();
		static void Draw();

		static void DrawCommandLog(const ImGuiViewport* Viewport, float HeightPosition);
		static void DrawCommandLine(const ImGuiViewport* Viewport, float HeightPosition);

		static void ExecuteCommand(const char* Command);
		static void RegisterCommand(const char* Command, std::function<void()> Function);

		static int InputCallback(void* data);

		static void AddToConsoleLog(const char* Log);

		struct Command
		{
			std::function<void()> Function;
		};

		static std::unordered_map<std::string, Command> Commands;
		static std::vector<std::string> RecentCommands;
		static std::vector<std::string> ConsoleLog;
		static int CurrentHistoryIndex;
		static bool ImguiDemoWindow;
		static int CurrentOpenState;
		static bool SetSrollToBottom;
		static int CommandlogHeight;
	};
}
