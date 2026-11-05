#include "ConsoleView.h"

#include <Core/Logger/DebugLogger.h>
#include <ImGui/imgui.h>

namespace Eclipse::Editor
{
	void ConsoleView::OnOpen()
	{

	}

	void ConsoleView::Draw()
	{
		const auto& messages = Core::DebugLogger::GetMessages();
		for (size_t i = 0; i < messages.Count(); i++)
		{
			const auto& mes = messages.Get(i);
			ImGui::Separator();
			ImGui::Text(mes.message.c_str());
		}
	}
}