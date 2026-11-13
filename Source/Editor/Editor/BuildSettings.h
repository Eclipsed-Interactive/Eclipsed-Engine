#pragma once

#include <Core/Math/Vector/Vector2.h>
#include "cereal/cereal.hpp"

namespace Eclipse::Settings
{
	struct BuildSettings
	{
		static constexpr const char* Name = "build";

		bool enableSteamSdk = false;
		bool enableNetworking = false;
		bool enableDiscordSdk = false;

		template<class Archive>
		void serialize(Archive& ar)
		{
			ar(CEREAL_NVP(enableSteamSdk));
			ar(CEREAL_NVP(enableNetworking));
			ar(CEREAL_NVP(enableDiscordSdk));
		}
	};

	BuildSettings& GetBuildSettings();
}