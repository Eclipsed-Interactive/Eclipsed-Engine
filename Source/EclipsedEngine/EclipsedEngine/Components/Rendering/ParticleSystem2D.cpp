#include "ParticleSystem2D.h"

#include "Core/Timer.h"

#include "Core/MainSingleton.h"
#include "Renderer/IRenderer.h"
#include "Renderer/IDrawer.h"
#include "Renderer/RenderCommands/CommandList.h"

#include "ECS/GameObject.h"
#include "EclipsedEngine/Components/Transform2D.h"

#include "Core/Math/Random.h"


void Eclipse::ParticleSystem2D::Awake()
{
	{
		Emitter& emitter = myEmmiter.emplace_back();
		emitter.SpawnPosition = { 2.f, 0.8f };
		emitter.SpawnShape = new CircleSpawnShape(0.25f);
	}

	{
		Emitter& emitter = myEmmiter.emplace_back();
		emitter.SpawnPosition = { -1.5, 0.f };
		emitter.SpawnShape = new RectSpawnShape({ 0.4f , 0.2f });
	}

	{
		Emitter& emitter = myEmmiter.emplace_back();
		emitter.SpawnPosition = { 3.5f, -1.5f };
	}
}

void Eclipse::ParticleSystem2D::Update()
{
	for (auto& Emitter : myEmmiter)
	{
		Emitter.ActivePosition = gameObject->transform->GetPosition() + Emitter.SpawnPosition;
		Emitter.Update();
	}
}




void Eclipse::Emitter::Init()
{

}

void Eclipse::Emitter::Update()
{
	if (CurrentTime >= SpawnInterval)
	{
		int particlesToSpawn = CurrentTime / SpawnInterval;

		for (int i = 0; i < particlesToSpawn; i++)
		{
			SpawnParticle();
		}

		CurrentTime = 0;
	}

	float deltaTime = Eclipse::Timer::GetDeltaTime();

	CurrentTime += deltaTime;


	{
		// TOFFLA: Just FPS display in outputwindow in Visual Studio
		std::stringstream sstream;
		sstream << 1.f / deltaTime << "\n";
		OutputDebugStringA(sstream.str().c_str());
	}



	for (int i = 0; i < myParticles.size(); i++)
	{
		Graphics::ParticleData& particle = myParticles[i];

		if (particle.LifeTime <= 0)
		{
			myParticles[i] = myParticles.back();
			myParticles.pop_back();
			i--;

			continue;
		}

		particle.LifeTime -= deltaTime;

		if (UseGravity)
			particle.Velocity.y -= deltaTime;

		particle.Position += particle.Velocity * deltaTime;
	}

	Graphics::CommandListManager* commandListManager = MainSingleton::GetPointer<Graphics::CommandListManager>();
	commandListManager->GetSpriteCommandList().Enqueue([&]()
		{
			Graphics::IRenderer& renderer = MainSingleton::GetInstance<Graphics::IRenderer>();
			Graphics::IDrawer* drawer = renderer.GetDrawer();

			drawer->DrawParticles(myParticles.data(), myParticles.size());
		});
}

void Eclipse::Emitter::SpawnParticle()
{
	Graphics::ParticleData particleData;

	Math::Vector2f SpawnShapePosition(0, 0);
	if (SpawnShape)
		SpawnShapePosition = SpawnShape->GetSpawnPosition();

	particleData.Position = ActivePosition + SpawnShapePosition;

	particleData.LifeTime = Math::Random::RandRange(1.8f, 2.5f);

	particleData.Velocity.x = Math::Random::RandRange(-0.5f, 0.5f);
	particleData.Velocity.y = Math::Random::RandRange(1.f, 1.5f);

	myParticles.push_back(particleData);
}
