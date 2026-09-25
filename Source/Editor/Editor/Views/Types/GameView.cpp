#include "GameView.h"

#include "Input/Input.h"

#include "ImGui/ImGui.h"

namespace Eclipse::Editor
{
	void GameView::OnOpen()
	{
		Input::Input::AddGameViewWindow(viewId);
	}
}