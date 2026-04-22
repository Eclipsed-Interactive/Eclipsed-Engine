#pragma once

#include "EclipsedEngine/Editor/Windows/WindowBase.h"


namespace Eclipse::Editor
{
	class InputEditorWindow final : public AbstractWindow
	{
		EDITOR_WINDOW_BASE_3(InputEditorWindow, "Input Editor", "Settings");

	public:
		void Update() override;

	public:
		bool isButtonEditing = false;
	};
}