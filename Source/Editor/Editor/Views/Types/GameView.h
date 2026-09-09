#pragma once

#include "Editor/Views/IEditorView.h"

namespace Eclipse::Editor
{
	class GameView : public EditorView<GameView>
	{
		BASIC_VIEW("Game")

	public:
		void Draw() override {}
	};
}