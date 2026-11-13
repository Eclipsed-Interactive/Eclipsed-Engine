#pragma once

#include <Core/Math/Vector/Vector2.h>
#include "cereal/cereal.hpp"

#include "Renderer.Core.hpp"

namespace Eclipse::Settings
{
	struct RENDERER_API GraphicsSettings
	{
		static constexpr const char* Name = "graphics";

		Math::Vector2<int> resolution = { 1280, 720 };
		bool fullscreen = false;
		int numberOfRenderBuffers = 0;
		bool renderLights = false;

		template<class Archive>
		void serialize(Archive& ar)
		{
			ar(
				CEREAL_NVP(resolution),
				CEREAL_NVP(fullscreen),
				CEREAL_NVP(numberOfRenderBuffers),
				CEREAL_NVP(renderLights)
			);
		}
	};

	RENDERER_API GraphicsSettings& GetGraphicsSettings();
}