#pragma once

#include "EclipsedEngine/Components/Component.h"

#include <vector>
#include "Renderer/ParticleDrawer.h"
#include "Core/Math/CommonMath.h"

/*

	TODO: Implement using SSBO/GPU particles

	Write to a buffer from a shader (SSBO, GL 4.3+)

	#version 430
	layout(local_size_x = 64) in;

	layout(std430, binding = 0) buffer Data 
	{
		vec4 values[];
	};

	void main() 
	{
		uint i = gl_GlobalInvocationID.x;
		values[i] *= 2.0;          // modifies the buffer in place
	}

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);
	glUseProgram(computeProgram);
	glDispatchCompute(count / 64, 1, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);  // required before other code reads it

	Fragment, vertex and compute shaders can all use SSBOs. Use glMemoryBarrier with a bit that matches the next use of the data:
	- GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT: the buffer is used as vertex data next.
	- GL_BUFFER_UPDATE_BARRIER_BIT: you read it back on the CPU.
	- GL_SHADER_STORAGE_BARRIER_BIT: another shader reads it.

 */

namespace Eclipse
{
	namespace Graphics
	{
		class ParticleData;
	}

	class BaseParticleSpawnShape
	{
	public:
		virtual Math::Vector2f GetSpawnPosition() = 0;
	};

	class CircleSpawnShape : public BaseParticleSpawnShape
	{
	public:
		~CircleSpawnShape() = default;

		CircleSpawnShape(float aRadius) : Radius(aRadius)
		{

		}

		virtual Math::Vector2f GetSpawnPosition() override
		{
			float RandomValue = Math::Random::RandRange(0.f, Math::pi2);

			float RandomDiStanceFromMiddle = Math::Random::RandRange(0.f, Radius);

			float RandomSpawnValueX = cos(RandomValue) * RandomDiStanceFromMiddle;
			float RandomSpawnValueY = sin(RandomValue) * RandomDiStanceFromMiddle;

			return Math::Vector2f(RandomSpawnValueX, RandomSpawnValueY);
		}

	private:

		float Radius;
	};

	class RectSpawnShape : public BaseParticleSpawnShape
	{
	public:
		~RectSpawnShape() = default;

		RectSpawnShape(Math::Vector2f aSize) : Size(aSize)
		{

		}

		virtual Math::Vector2f GetSpawnPosition() override
		{
			float RandomSpawnValueX = Math::Random::RandRange(-Size.x, Size.x);
			float RandomSpawnValueY = Math::Random::RandRange(-Size.y, Size.y);

			return Math::Vector2f(RandomSpawnValueX, RandomSpawnValueY);
		}

	private:

		Math::Vector2f Size;
	};

	class Emitter
	{
	public:
		void Init();
		void Update();

		void SpawnParticle();

		std::vector<Graphics::ParticleData> myParticles;

		BaseParticleSpawnShape* SpawnShape = nullptr;

		bool UseGravity = true;
		float SpawnInterval = 0.005f;
		float CurrentTime = 0;
		Math::Vector2f SpawnPosition = {0, 0};
		Math::Vector2f ActivePosition = {0, 0};
	};

	class ParticleSystem2D : public Component
	{
	public:
		ParticleSystem2D() = default;
		~ParticleSystem2D() = default;

		void Awake() override;
		void Update() override;

	private:
		std::vector<Emitter> myEmmiter;
	};
}
