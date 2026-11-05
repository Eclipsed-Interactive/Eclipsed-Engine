#pragma once

#include "Editor/Views/IEditorView.h"

namespace Eclipse::Editor
{
	class ConsoleView : public EditorView<ConsoleView>
	{
		BASIC_VIEW("Console")

	public:
		void OnOpen() override;
		void Draw() override;
	};
}