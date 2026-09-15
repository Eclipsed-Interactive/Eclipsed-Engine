#pragma once

#include "SettingsBase.hpp"
#include "Core/Math/Vector/Vector2.h"

namespace Eclipse::Settings
{
	struct BuildSettings : public BaseSettings<BuildSettings>
	{
		SETTINGS_DATA{
			//std::vector<std::string> SceneIndex;
			bool enableSteamSdk = false;
			bool enableNetworking = false;
			bool enableDiscordSdk = false;

			SERIALIZE(
				//MAKE_NVP(SceneIndex),
				MAKE_NVP(enableSteamSdk),
				MAKE_NVP(enableNetworking),
				MAKE_NVP(enableDiscordSdk)
			)
		};

		//GET_SET(SceneIndex);
		GET_SET(enableSteamSdk);
		GET_SET(enableNetworking);
		GET_SET(enableDiscordSdk);

		BASE_SETTINGS(BuildSettings, "build");
	};
}