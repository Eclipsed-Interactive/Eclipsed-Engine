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
	{
		Emitter& emitter2 = myEmmiter.emplace_back();
		emitter2.SpawnPosition = { 6.f, 3.8f };
		Emitter& emitter1 = myEmmiter.emplace_back();
		emitter1.SpawnPosition = { -4.f, -2.f };
		Emitter& emitter = myEmmiter.emplace_back();
		emitter.SpawnPosition = { 2.f, -0.8f };

		emitter.SpawnShape = new CircleSpawnShape(0.25f);

		emitter.StartData.StartLifetimeMin = 1.8f;
		emitter.StartData.StartLifetimeMax = 2.5f;

		emitter.StartData.StartVelocityMin.x = -0.5f;
		emitter.StartData.StartVelocityMin.y = 1.f;

		emitter.StartData.StartVelocityMax.x = 0.5f;
		emitter.StartData.StartVelocityMax.y = 1.5f;
	}

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
}

void Eclipse::ParticleSystem2D::Render()
{
	Graphics::CommandListManager* commandListManager = MainSingleton::GetPointer<Graphics::CommandListManager>();
	commandListManager->GetSpriteCommandList().Enqueue([&]()
		{
			Graphics::IRenderer& renderer = MainSingleton::GetInstance<Graphics::IRenderer>();
			Graphics::IDrawer* drawer = renderer.GetDrawer();

			drawer->DrawParticles();
		});
}

void Eclipse::ParticleSystem2D::Update()
{
	for (auto& Emitter : myEmmiter)
	{
		Emitter.ActivePosition = gameObject->transform->GetPosition() + Emitter.SpawnPosition;
		Emitter.Update();
	}


	Graphics::IRenderer& renderer = MainSingleton::GetInstance<Graphics::IRenderer>();
	Graphics::IGraphicsBuffer* GraphicsBuffer = renderer.GetGraphicsBuffer();
	Graphics::IComputer* computer = renderer.GetComputer();

	SharedGPUParticlesBuffer* sharedGpuParticleBuffer;
	GraphicsBuffer->GetBuffer<SharedGPUParticlesBuffer>(sharedGpuParticleBuffer);

	float deltaTime = Eclipse::Timer::GetDeltaTime();

	sharedGpuParticleBuffer->Gravity = Math::Vector3f(0, -1, 0.f);
	sharedGpuParticleBuffer->DeltaTime = deltaTime;
	sharedGpuParticleBuffer->FrameSeed++;

	GraphicsBuffer->SetOrCreateBuffer<SharedGPUParticlesBuffer>(1);

	computer->ComputeParticleUpdate();
}

void Eclipse::ParticleSystem2D::OnDestroy()
{
	for (auto& Emitter : myEmmiter)
	{
		delete Emitter.SpawnShape;
	}
}

void Eclipse::Emitter::Update()
{
	Graphics::IRenderer& renderer = MainSingleton::GetInstance<Graphics::IRenderer>();
	Graphics::IGraphicsBuffer* GraphicsBuffer = renderer.GetGraphicsBuffer();
	Graphics::IComputer* computer = renderer.GetComputer();

	float deltaTime = Eclipse::Timer::GetDeltaTime();
	CurrentTime += SpawnInterval * deltaTime;
	unsigned ToSpawn = (unsigned)CurrentTime;
	CurrentTime -= ToSpawn;

	SharedGPUParticlesBuffer* sharedGpuParticleBuffer;
	GraphicsBuffer->GetBuffer<SharedGPUParticlesBuffer>(sharedGpuParticleBuffer);

	sharedGpuParticleBuffer->EmitterPosition = Math::Vector3f(ActivePosition.x, ActivePosition.y, 0.f);
	sharedGpuParticleBuffer->SpawnCount = ToSpawn;

	GraphicsBuffer->SetOrCreateBuffer<SharedGPUParticlesBuffer>(1);

	if (ToSpawn)
		computer->ComputeParticleSpawning(ToSpawn);
}