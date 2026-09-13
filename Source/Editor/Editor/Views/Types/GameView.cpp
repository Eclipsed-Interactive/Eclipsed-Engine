#include "GameView.h"

#include "Input/Input.h"

namespace Eclipse::Editor
{
	void GameView::OnOpen()
	{
		Input::Input::AddGameViewWindow(viewId);
	}
}