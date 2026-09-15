#pragma once

#include "SettingsBase.hpp"
#include <array>

namespace Eclipse::Settings
{
	struct PhysicsSettings final : public BaseSettings<PhysicsSettings>
	{
		#define MAX_LAYERS 16

		SETTINGS_DATA{
		std::array<unsigned long long, MAX_LAYERS> PhysicsLayers{255};

			SERIALIZE(
				MAKE_NVP(PhysicsLayers)
			)
		};

		GET_SET(PhysicsLayers)

		BASE_SETTINGS(PhysicsSettings, "physics");
	};
}