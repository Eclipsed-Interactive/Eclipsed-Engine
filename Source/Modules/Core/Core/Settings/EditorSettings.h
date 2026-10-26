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

		EditorSettings() = default; ~EditorSettings() = default; struct TEMP {
			TEMP() {
				Eclipse::EventSystem::Subscribe("Engine-Load", EditorSettings::Load); Eclipse::EventSystem::Subscribe("Engine-Shutdown", EditorSettings::Save);
			}
		}; static inline TEMP temp = {}; static inline EditorSettings::Data& GetData() {
			static EditorSettings::Data data{}; return data;
		} static constexpr const char* Name = "editor"; static constexpr const char* SettingsName() {
			return EditorSettings::Name;
		}
	};
}