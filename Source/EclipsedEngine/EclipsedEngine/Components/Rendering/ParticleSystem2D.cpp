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
#include "Core/GraphicsBuffers/EmitterParticlesBuffer.h"
#include "Core/GraphicsBuffers/ParticleSpawnBuffer.h"

#include <Renderer/IGraphicsBuffer.h>



void Eclipse::ParticleSystem2D::Awake()
{
	Graphics::IRenderer& renderer = MainSingleton::GetInstance<Graphics::IRenderer>();
	Graphics::IGraphicsBuffer* GraphicsBuffer = renderer.GetGraphicsBuffer();

	GraphicsBuffer->GetBuffer<ParticleSpawnBuffer>(ParticleSpawnGpuParticleBuffer);
	GraphicsBuffer->GetBuffer<ConeEmitterDataBuffer>(ConeEmitterParticleBuffer);
	GraphicsBuffer->GetBuffer<SharedGPUParticlesBuffer>(SharedParticleBuffer);

	Graphics::IComputer* computer = renderer.GetComputer();
	computer->ComputeDestroyAllParticles();

	{
		Emitter& emitter = myEmmiter.emplace_back();
		emitter.SpawnPosition = { 0.8f, -0.4f };
		emitter.Gravity = { 0.f, 0.f };

		emitter.SpawnShape = 1;

		emitter.StartData.StartLifetimeMin = 1.f;
		emitter.StartData.StartLifetimeMax = 1.4f;

		emitter.StartData.StartSizeMin = 0.02f;
		emitter.StartData.StartSizeMax = 0.02f;

		emitter.StartData.EndSizeMin = 0.005f;
		emitter.StartData.EndSizeMax = 0.005f;

		emitter.StartData.StartSpeedMin = 0.8f;
		emitter.StartData.StartSpeedMax = 1.4f;
	}

	{
		Emitter& emitter = myEmmiter.emplace_back();
		emitter.SpawnPosition = { -1., 0.f };


		emitter.SpawnShape = 0;

		emitter.StartData.StartLifetimeMin = 2.f;
		emitter.StartData.StartLifetimeMax = 2.f;

		emitter.StartData.StartSizeMin = 0.03f;
		emitter.StartData.StartSizeMax = 0.03f;

		emitter.StartData.EndSizeMin = 0.03f;
		emitter.StartData.EndSizeMax = 0.03f;

		emitter.StartData.StartSpeedMin = 1.f;
		emitter.StartData.StartSpeedMax = 1.4f;
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

			if (Texture0.dataPtr)
				renderer.GetDevice()->BindTexture(0, Texture0);
			if (Texture1.dataPtr)
				renderer.GetDevice()->BindTexture(1, Texture1);
			if (Texture2.dataPtr)
				renderer.GetDevice()->BindTexture(2, Texture2);
			if (Texture3.dataPtr)
				renderer.GetDevice()->BindTexture(3, Texture3);
			if (Texture4.dataPtr)
				renderer.GetDevice()->BindTexture(4, Texture4);
			if (Texture5.dataPtr)
				renderer.GetDevice()->BindTexture(5, Texture5);
			if (Texture6.dataPtr)
				renderer.GetDevice()->BindTexture(6, Texture6);
			if (Texture7.dataPtr)
				renderer.GetDevice()->BindTexture(7, Texture7);

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

	float deltaTime = Eclipse::Timer::GetDeltaTime();

	// Update
	SharedParticleBuffer->DeltaTime = deltaTime;

	// Spawning
	ParticleSpawnGpuParticleBuffer->FrameSeed++;

	GraphicsBuffer->SetOrCreateBuffer<SharedGPUParticlesBuffer>(1);

	computer->ComputeParticleUpdate();
}

void Eclipse::ParticleSystem2D::OnDestroy()
{
	//for (auto& Emitter : myEmmiter)
	//{
	//	delete Emitter.SpawnShape;
	//}

	Graphics::IRenderer& renderer = MainSingleton::GetInstance<Graphics::IRenderer>();
	Graphics::IComputer* computer = renderer.GetComputer();

	computer->ComputeDestroyAllParticles();
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

	if (ToSpawn)
	{
		ParticleSystem2D::ConeEmitterParticleBuffer->EmitterRotation = 0.f;
		ParticleSystem2D::ConeEmitterParticleBuffer->EmitterWidthAngle = 30.f;
		ParticleSystem2D::ConeEmitterParticleBuffer->MinSpeed = 1.f;
		ParticleSystem2D::ConeEmitterParticleBuffer->MaxSpeed = 1.4f;


		ParticleSystem2D::ParticleSpawnGpuParticleBuffer->Gravity = Math::Vector3f(Gravity.x, Gravity.y, 0.f);
		ParticleSystem2D::ParticleSpawnGpuParticleBuffer->EmitterPosition = Math::Vector3f(ActivePosition.x, ActivePosition.y, 0.f);
		ParticleSystem2D::ParticleSpawnGpuParticleBuffer->SpawnCount = ToSpawn;
		ParticleSystem2D::ParticleSpawnGpuParticleBuffer->SpawnShape = SpawnShape;

		ParticleSystem2D::ParticleSpawnGpuParticleBuffer->SpawnLifetimeMin = StartData.StartLifetimeMin;
		ParticleSystem2D::ParticleSpawnGpuParticleBuffer->SpawnLifetimeMax = StartData.StartLifetimeMax;

		ParticleSystem2D::ParticleSpawnGpuParticleBuffer->MinSizeStart = StartData.StartSizeMin;
		ParticleSystem2D::ParticleSpawnGpuParticleBuffer->MaxSizeStart = StartData.StartSizeMax;
		ParticleSystem2D::ParticleSpawnGpuParticleBuffer->MinSizeEnd = StartData.EndSizeMin;
		ParticleSystem2D::ParticleSpawnGpuParticleBuffer->MaxSizeEnd = StartData.EndSizeMax;

		GraphicsBuffer->SetOrCreateBuffer<ParticleSpawnBuffer>(0);
		GraphicsBuffer->SetOrCreateBuffer<ConeEmitterDataBuffer>(2);

		computer->ComputeParticleSpawning(ToSpawn);
	}
}