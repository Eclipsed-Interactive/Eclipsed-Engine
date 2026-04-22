#pragma once

#include "SettingsBase.hpp"
#include "CoreEngine/Math/Vector/Vector2.h"

namespace Eclipse::Settings
{
	struct BuildSettings : public BaseSettings<BuildSettings>
	{
		SETTINGS_DATA{
			std::vector<std::string> SceneIndex;

			SERIALIZE(MAKE_NVP(SceneIndex))
		};

		GET_SET(SceneIndex)

		BASE_SETTINGS(BuildSettings, "build");
	};
}