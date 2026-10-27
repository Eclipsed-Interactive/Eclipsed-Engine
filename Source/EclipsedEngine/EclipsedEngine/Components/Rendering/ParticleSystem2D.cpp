#include "ParticleSystem2D.h"

#include "Core/Timer.h"

#include "Core/MainSingleton.h"
#include "Renderer/IRenderer.h"
#include "Renderer/IDrawer.h"
#include "Renderer/RenderCommands/CommandList.h"

#include "ECS/GameObject.h"
#include "EclipsedEngine/Components/Transform2D.h"

#include "Core/Math/Random.h"

#include "Core/GraphicsBuffers/SharedGPUParticlesBuffer.h"

#include <Renderer/IGraphicsBuffer.h>


void Eclipse::ParticleSystem2D::Awake()
{
	//{
	//	Emitter& emitter = myEmmiter.emplace_back();
	//	emitter.SpawnPosition = { 2.f, 0.8f };
	//	emitter.SpawnShape = new CircleSpawnShape(0.25f);

	//	emitter.StartData.StartLifetimeMin = 1.8f;
	//	emitter.StartData.StartLifetimeMax = 2.5f;

	//	emitter.StartData.StartVelocityMin.x = -0.5f;
	//	emitter.StartData.StartVelocityMin.y = 1.f;

	//	emitter.StartData.StartVelocityMax.x = 0.5f;
	//	emitter.StartData.StartVelocityMax.y = 1.5f;
	//}

	{
		Emitter& emitter = myEmmiter.emplace_back();
		emitter.SpawnPosition = { -1.5, 0.f };
		emitter.SpawnShape = new RectSpawnShape({ 0.4f , 0.2f });

		emitter.StartData.StartLifetimeMin = 2.f;
		emitter.StartData.StartLifetimeMax = 2.f;

		emitter.StartData.StartVelocityMin.x = -1.5f;
		emitter.StartData.StartVelocityMin.y = 1.f;

		emitter.StartData.StartVelocityMax.x = -1.f;
		emitter.StartData.StartVelocityMax.y = 2.f;
	}

	//{
	//	Emitter& emitter = myEmmiter.emplace_back();
	//	emitter.SpawnPosition = { 3.5f, -1.5f };
	//	emitter.SpawnShape = new CircleSpawnShape(0.2f);
	//	emitter.EmmiterShape = new ShpericalEmitterShape();

	//	emitter.StartData.StartLifetimeMin = 2.2f;
	//	emitter.StartData.StartLifetimeMax = 2.5f;

	//	emitter.StartData.StartVelocityMin.x = 1.f;
	//	emitter.StartData.StartVelocityMin.y = 1.f;

	//	emitter.StartData.StartVelocityMax.x = 1.f;
	//	emitter.StartData.StartVelocityMax.y = 1.f;

	//	emitter.Gravity = { 0.f, 0.f};
	//}

	for (auto& Emitter : myEmmiter)
	{
		Emitter.Init();
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

void Eclipse::ParticleSystem2D::OnDestroy()
{
	for (auto& Emitter : myEmmiter)
	{
		delete Emitter.SpawnShape;
	}
}

void Eclipse::Emitter::Init()
{
	Graphics::IRenderer& renderer = MainSingleton::GetInstance<Graphics::IRenderer>();
	Graphics::IComputer* computer = renderer.GetComputer();

	SSBOID = computer->CreateNewShaderStorageBufferObject(myParticles.data(), myParticles.size());
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


	Graphics::IRenderer& renderer = MainSingleton::GetInstance<Graphics::IRenderer>();
	Graphics::IGraphicsBuffer* GraphicsBuffer = renderer.GetGraphicsBuffer();

	SharedGPUParticlesBuffer* sharedGpuParticleBuffer;
	GraphicsBuffer->GetBuffer<SharedGPUParticlesBuffer>(sharedGpuParticleBuffer);

	sharedGpuParticleBuffer->DeltaTime = deltaTime;
	sharedGpuParticleBuffer->Gravity = Gravity;

	GraphicsBuffer->SetOrCreateBuffer<SharedGPUParticlesBuffer>(1);


	Graphics::IComputer* computer = renderer.GetComputer();
	computer->ComputeParticleUpdate(myParticles.data(), myParticles.size(), SSBOID);

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
	}





	/*	for (int i = 0; i < myParticles.size(); i++)
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

			particle.Velocity.y -= deltaTime;

		particle.Position += particle.Velocity * deltaTime;
	}*/







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

	particleData.Position = ActivePosition + SpawnShapePosition + StartData.StartPosition;

	particleData.LifeTime = 3.f;//Math::Random::RandRange(StartData.StartLifetimeMin, StartData.StartLifetimeMax);

	if (EmmiterShape)
	{
		Math::Vector2f Veclocity = EmmiterShape->GetStartVelocity();

		particleData.Velocity.x = Veclocity.x * Math::Random::RandRange(StartData.StartVelocityMin.y, StartData.StartVelocityMax.y);
		particleData.Velocity.y = Veclocity.y * Math::Random::RandRange(StartData.StartVelocityMin.y, StartData.StartVelocityMax.y);
	}
	else
	{
		particleData.Velocity.x = Math::Random::RandRange(StartData.StartVelocityMin.x, StartData.StartVelocityMax.x);
		particleData.Velocity.y = Math::Random::RandRange(StartData.StartVelocityMin.y, StartData.StartVelocityMax.y);
	}


	myParticles.push_back(particleData);
}
