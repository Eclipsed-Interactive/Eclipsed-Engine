#pragma once

#include <vector>
#include <string>

#include "SettingsBase.hpp"
#include "CoreEngine/Math/Vector/Vector2.h"

namespace Eclipse::Settings
{
	struct OpenEditorWindows
	{
		int ID;
		std::string Name;


		SERIALIZE(
			MAKE_NVP(ID),
			MAKE_NVP(Name)
		)
	};

	struct EditorSettings : public BaseSettings<EditorSettings>
	{
		SETTINGS_DATA{
			std::string LastActiveScene = "";
			std::vector<OpenEditorWindows> CurrentlyOpenEditorWindows;

			SERIALIZE(
				MAKE_NVP(LastActiveScene),
				MAKE_NVP(CurrentlyOpenEditorWindows)
			)
		};

		GET_SET(LastActiveScene)
		GET_SET(CurrentlyOpenEditorWindows)

			BASE_SETTINGS(EditorSettings, "editor");
	};
}