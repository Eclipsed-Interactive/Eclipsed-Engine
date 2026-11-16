#pragma once

#include "Editor/Views/IEditorView.h"

#include "Settings/InputSettingsPanel.h"
#include "Settings/GraphicsSettingsPanel.h"
#include "Settings/EditorSettingsPanel.h"

namespace Eclipse::Editor
{
	class SettingsView : public EditorView<SettingsView>
	{
		BASIC_VIEW("Settings")

	public:
		void OnOpen() override;
		void Draw() override;

	private:
		InputSettingsPanel inputSettings;
		GraphicsSettingsPanel graphicsSettings;
		EditorSettingsPanel editorSettings;
 	};
}