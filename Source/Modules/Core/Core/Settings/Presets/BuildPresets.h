#pragma once

#include "Core/Settings/Config.h"
#include "Core/Math/Vector/Vector2.h"


namespace Eclipse::Configs
{
	class BuildPresets
	{
	public:
		static inline Config Default()
		{
			Config config;
			config.Add<bool>("enableSteamSdk", false);
			config.Add<bool>("enableDiscordSdk", false);
			config.Add<bool>("enableNetworking", false);
			config.Add<Math::Vector2f>("gravity", {0.f, -9.82f});
			config.Add<std::string>("name", "Game Name");
			config.Add<const char*>("simon", "noah");

			return config;
		}
	};
}