#pragma once

#include "EclipsedEngine/Editor/Windows/WindowBase.h"
#include "CoreEngine/Debug/DebugMessage.h"
#include "CoreEngine/Debug/DebugLogger.h"

namespace Eclipse::Editor
{
	class ConsoleWindow final : public AbstractWindow 
	{
		EDITOR_WINDOW_BASE_2(ConsoleWindow, "Console")

	public:
		void Update() override;

	private:
		void DisplayMessage(const DebugMessage& aMessage);

	private:
		bool myAlwaysAtBottom = false;
		bool myShouldCollapseMessages = false;
		bool myShouldShowMessages = true;
		bool myShouldShowWarnings = true;
		bool myShouldShowErrors = true;
	};
}