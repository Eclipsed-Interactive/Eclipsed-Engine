#pragma once

#include <Core/Math/Vector/Vector2.h>
#include "cereal/cereal.hpp"
#include "cereal/types/array.hpp"
#include "PhysicsEngineSettings.h"

#include "Physics.Core.hpp"

namespace Eclipse::Settings
{
	struct PHYSICS_API PhysicsSettings
	{
		static constexpr const char* Name = "physics";

		Math::Vector2f gravity;
		std::array<uint64_t, MAX_LAYERS> PhysicsLayers = DefaultLayers();

		template<class Archive>
		void serialize(Archive& ar)
		{
			ar(
				CEREAL_NVP(gravity),
				CEREAL_NVP(PhysicsLayers)
			);
		}

		static constexpr std::array<uint64_t, MAX_LAYERS> DefaultLayers()
		{
			std::array<uint64_t, MAX_LAYERS> layers{};
			for (auto& layer : layers) layer = 255;
			return layers;
		}
	};

	PHYSICS_API PhysicsSettings& GetPhysicsSettings();
}