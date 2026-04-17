#pragma once

#include "SettingsBase.hpp"
#include <array>

namespace Eclipse::Settings
{
	struct PhysicsSettings : public BaseSettings<PhysicsSettings>
	{
		#define MAX_LAYERS 16

		SETTINGS_DATA{
		std::array<unsigned long long, MAX_LAYERS> PhysicsLayers;

			SERIALIZE(
				MAKE_NVP(PhysicsLayers)
			)
		};

		GET_SET(PhysicsLayers)

		BASE_SETTINGS(PhysicsSettings, "physics");
	};
}