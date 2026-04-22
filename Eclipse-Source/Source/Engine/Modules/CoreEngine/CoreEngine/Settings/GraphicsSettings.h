#pragma once

#include "SettingsBase.hpp"
#include "CoreEngine/Math/Vector/Vector2.h"

namespace Eclipse::Settings
{
	struct GraphicsSettings : public BaseSettings<GraphicsSettings>
	{
		SETTINGS_DATA{
			Math::Vector2<int> Resolution = { 1280, 720 };
			bool Fullscreen = false;
			int NumberOfRenderBuffers = 0;

			SERIALIZE(
				MAKE_NVP(Resolution),
				MAKE_NVP(Fullscreen),
				MAKE_NVP(NumberOfRenderBuffers)
				)
		};

		GET_SET(Resolution)
		GET_SET(Fullscreen)
		GET_SET(NumberOfRenderBuffers)

		BASE_SETTINGS(GraphicsSettings, "graphics");
	};
}