#pragma once

#include "Editor/Views/IEditorView.h"

namespace Eclipse::Editor
{
	class SettingsView : public EditorView<SettingsView>
	{
		BASIC_VIEW("Settings")

	public:
		void OnOpen() override;
		void Draw() override;

	private:
		void DrawInputDebug();
		void DrawInputMapper();

		void DrawInputMapperPopups();

	private:
		void SaveMapper(const char* mapPath);
		void LoadMapper(const char* mapPath);

	public:
		int selectedMap = 0;
		std::string mapName = "";
		std::string actionName = "";
		std::string axisName;

		enum class AxisBinding { Negative, Positive };
		AxisBinding listeningAxisBinding = AxisBinding::Negative;

		bool mapChanged = false;

		int axisSelection = 0;
		int actionSelection = 0;
		enum class SelectionType
		{
			Axis, Action
		} type;

	};
}