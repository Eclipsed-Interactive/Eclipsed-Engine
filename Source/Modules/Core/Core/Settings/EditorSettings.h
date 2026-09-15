#pragma once

#include <vector>
#include <string>

#include "SettingsBase.hpp"
#include "Core/Math/Vector/Vector2.h"

namespace Eclipse::Settings
{
	struct EditorSettings final : public BaseSettings<EditorSettings>
	{
		struct OpenEditorWindows final
		{
			int ID;
			std::string Name;

			SERIALIZE(
				MAKE_NVP(ID),
				MAKE_NVP(Name)
			)
		};

		SETTINGS_DATA{
			std::string LastActiveScene = "";
			std::vector<OpenEditorWindows> CurrentlyOpenEditorWindows;
			bool OpenLastOpenSceneOnStartup = true;

			SERIALIZE(
				MAKE_NVP(LastActiveScene),
				MAKE_NVP(CurrentlyOpenEditorWindows),
				MAKE_NVP(OpenLastOpenSceneOnStartup)
			)
		};

		GET_SET(LastActiveScene);
		GET_SET(CurrentlyOpenEditorWindows);
		GET_SET(OpenLastOpenSceneOnStartup);

		BASE_SETTINGS(EditorSettings, "editor")
	};
}