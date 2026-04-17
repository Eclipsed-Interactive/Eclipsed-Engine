#pragma once

#include "EclipsedEngine/Editor/Windows/WindowBase.h"


namespace Eclipse::Editor
{
	class GameSettingsWindow final : public AbstractWindow
	{
		EDITOR_WINDOW_BASE_3(GameSettingsWindow, "Game Settings", "Settings");

	public:
		void Update() override;

	private:
		void DrawSceneEditor();
		void DrawCollisionLayerEditor();
		void SaveLayerEditToJSON();
	};
}