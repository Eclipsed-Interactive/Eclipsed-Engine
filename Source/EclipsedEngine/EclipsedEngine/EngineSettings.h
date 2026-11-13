#pragma once

#include <Core/Math/Vector/Vector2.h>
#include "cereal/cereal.hpp"

#include "EclipsedEngine.Core.hpp"

namespace Eclipse::Settings
{
	struct ECL_API EngineSettings
	{
		static constexpr const char* Name = "engine";


		std::string GameVersion = "0.0.0";

		template<class Archive>
		void serialize(Archive& ar)
		{
			ar(
				CEREAL_NVP(GameVersion)
			);
		}
	};

	ECL_API EngineSettings& GetEngineSettings();
}