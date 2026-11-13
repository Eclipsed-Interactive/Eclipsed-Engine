#pragma once

#include "Core/Math/Vector/Vector3.h"
#include "Core/Math/Color.h"

namespace Eclipse
{
	namespace Particles
	{
		const unsigned maxParticles = 10'000;

		struct ParticleData
		{
			Math::Vector3f Position;
			float Size;

			Math::Vector3f Velocity;
			float LifeTime;

			int Color;
			int StartColor;
			int EndColor;
			int pad6;

			Math::Vector3f Gravity;
			float TextureIndex;

			float pad0, pad1, pad2, pad3; // Change over time

			float MaxLife, Rotation, pad4, pad5;
		};
	}
}