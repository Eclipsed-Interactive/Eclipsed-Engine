#pragma once

#include "Core/Math/Vector/Vector3.h"
#include "Core/Math/Color.h"

namespace Eclipse
{
	namespace Particles
	{
		const unsigned maxParticles = 100'000;

		struct ParticleData
		{
			Math::Vector3f Position;
			float Size;

			Math::Vector3f Velocity;
			float LifeTime;

			Math::Color color;

			float MaxLife, Rotation, pad0, pad1;
		};
	}
}