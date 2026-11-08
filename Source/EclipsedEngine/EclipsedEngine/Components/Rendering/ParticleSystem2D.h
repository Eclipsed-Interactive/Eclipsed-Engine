#pragma once

#include "EclipsedEngine/Components/Component.h"

#include <vector>
#include "Renderer/ParticleDrawer.h"
#include "Core/Math/CommonMath.h"

#include <array>

#include "Assets/Assets/TextureAsset.h"

namespace Eclipse
{
	namespace Graphics
	{
		class ParticleData;
	}

	class BaseParticleEmittingShape
	{
	public:
		virtual Math::Vector2f GetStartVelocity() = 0;
	};

	class ShpericalEmitterShape : public BaseParticleEmittingShape
	{
	public:
		ShpericalEmitterShape() = default;
		~ShpericalEmitterShape() = default;

		virtual Math::Vector2f GetStartVelocity() override
		{
			float RandomValue = Math::Random::RandRange(0.f, Math::pi2);

			float RandomVelocityValueX = cos(RandomValue);
			float RandomVelocityValueY = sin(RandomValue);

			return Math::Vector2f(RandomVelocityValueX, RandomVelocityValueY);
		}
	};




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
		void Update();

		//std::vector<Graphics::ParticleData> myParticles;

		Math::Vector2f Gravity = {0, -1.f};

		float SpawnInterval = 100.f;
		float CurrentTime = 0;
		Math::Vector2f SpawnPosition = {0, 0};
		Math::Vector2f ActivePosition = {0, 0};

		unsigned SpawnShape = 0;
		
		struct ParticleStartData
		{
			Math::Vector2f StartPosition = { 0.f, 0.f };

			float StartSpeedMin = 1.f;
			float StartSpeedMax = 1.f;

			float StartLifetimeMin = 1.f;
			float StartLifetimeMax = 1.f;

			float StartSizeMin = 0.02f;
			float StartSizeMax = 0.02f;

			float EndSizeMin = 0.02f;
			float EndSizeMax = 0.02f;
		} StartData;
	};

	class ParticleSystem2D : public Component
	{
		MAKE_REFLECTABLE(ParticleSystem2D);
		REFLECTION_FLAG(COMPONENT);

		COMPONENT_BASE_2(ParticleSystem2D, UINT_MAX * 0.5f);

	public:
		void Awake() override;
		void Render() override;
		void Update() override;

		void OnDestroy() override;

	private:
		std::vector<Emitter> myEmmiter;

	public:
		static inline class ParticleSpawnBuffer* ParticleSpawnGpuParticleBuffer;
		static inline class ConeEmitterDataBuffer* ConeEmitterParticleBuffer;
		static inline class SharedGPUParticlesBuffer* SharedParticleBuffer;

		[[SerializeField]]
		Assets::Texture Texture0;
		[[SerializeField]]
		Assets::Texture Texture1;
		[[SerializeField]]
		Assets::Texture Texture2;
		[[SerializeField]]
		Assets::Texture Texture3;
		[[SerializeField]]
		Assets::Texture Texture4;
		[[SerializeField]]
		Assets::Texture Texture5;
		[[SerializeField]]
		Assets::Texture Texture6;
		[[SerializeField]]
		Assets::Texture Texture7;

	};
}
