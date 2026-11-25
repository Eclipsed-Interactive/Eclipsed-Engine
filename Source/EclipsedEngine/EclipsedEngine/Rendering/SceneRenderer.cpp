#include "SceneRenderer.h"

#include "Renderer/RendererManager.h"
#include "Renderer/IRenderer.h"

#include "Renderer/RenderCommands/CommandList.h"
#include "Core/MainSingleton.h"

#include "EclipsedEngine/Components/Rendering/Camera.h"
#include "EclipsedEngine/Components/Transform2D.h"
#include <Renderer/GraphicsSettings.h>

#include <EclipsedEngine/Components/Rendering/Lights/DirectionalLight2D.h>
#include <Core/MainSingleton.h>

namespace Eclipse::Rendering
{
	void SceneRenderer::Init()
	{
		GetMaterials();
		CreateBuffers();
	}

	bool SceneRenderer::Render()
	{
		if (!Camera::MainCamera)
			return false;

		UpdateCameraBuffer();

		RenderSprites();

		if (Settings::GetGraphicsSettings().renderLights)
		{
			RenderLights();
			
			if (MainSingleton::Exists<DirectionalLight2D>())
			{
				RenderShadowSprites();
			}

			RenderComposite();
		}

		return true;
	}

	void SceneRenderer::CreateBuffers()
	{
		Graphics::IGraphicsDevice* device = Graphics::RendererManager::GetRenderer().GetDevice();

		GameFrameBuffer = device->CreateFrameBuffer(true);
		LightsFrameBuffer = device->CreateFrameBuffer(true);
		ShadowsFrameBuffer = device->CreateFrameBuffer(true);
	}

	void SceneRenderer::GetMaterials()
	{
		//LightCompositeMat = Assets::AssetManager::LoadDefault<Assets::Material>(Assets::DefaultAssetType::MATERIAL_2D_LIGHTS_COMPOSITE);
	}

	void SceneRenderer::RenderLights()
	{
		Eclipse::Graphics::IGraphicsDevice* device = Graphics::RendererManager::GetRenderer().GetDevice();
		Graphics::CommandListManager* commandListManager = MainSingleton::GetPointer<Graphics::CommandListManager>();

		device->BindFrameBuffer(LightsFrameBuffer.frameBufferIndex);
		device->Clear(Graphics::ClearFlags::Color, { 0.f, 0.f, 0.f, 1.f });
		device->SetBlendMode(Graphics::BlendMode::Additive);
		commandListManager->GetPointLightCommandList().Execute();
		commandListManager->GetSpotLightCommandList().Execute();

		device->SetBlendMode(Graphics::BlendMode::Alpha);
	}

	void SceneRenderer::RenderSprites()
	{
		Eclipse::Graphics::IGraphicsDevice* device = Graphics::RendererManager::GetRenderer().GetDevice();

		Graphics::CommandListManager* commandListManager = MainSingleton::GetPointer<Graphics::CommandListManager>();
		device->BindFrameBuffer(GameFrameBuffer.frameBufferIndex);
		device->Clear(Graphics::ClearFlags::Color, Camera::MainCamera->ClearColor);
		commandListManager->GetSpriteCommandList().Execute();
	}

	void SceneRenderer::RenderShadowSprites()
	{
		Eclipse::Graphics::IGraphicsDevice* device = Graphics::RendererManager::GetRenderer().GetDevice();

		Graphics::CommandListManager* commandListManager = MainSingleton::GetPointer<Graphics::CommandListManager>();
		device->BindFrameBuffer(ShadowsFrameBuffer.frameBufferIndex);
		device->Clear(Graphics::ClearFlags::Color, { 0.f, 0.f, 0.f, 0.f });
		commandListManager->GetSpriteCommandList().Execute();
	}

	void SceneRenderer::RenderComposite()
	{
		Eclipse::Graphics::IGraphicsDevice* device = Graphics::RendererManager::GetRenderer().GetDevice();
		Graphics::CommandListManager* commandListManager = MainSingleton::GetPointer<Graphics::CommandListManager>();

		device->BindFrameBuffer(GameFrameBuffer.frameBufferIndex);
		device->SetBlendMode(Graphics::BlendMode::Multiply);
		LightCompositeMat.Bind();
		device->BindTexture(0, LightsFrameBuffer.textureIndex);
		device->BindTexture(1, ShadowsFrameBuffer.textureIndex);
		auto* drawer = Graphics::RendererManager::GetRenderer().GetDrawer();
		drawer->DrawSprite();

		device->SetBlendMode(Graphics::BlendMode::Alpha);
	}

	void SceneRenderer::UpdateCameraBuffer()
	{
		if (!MainSingleton::Exists<Camera>())
			return;

		if (!Camera::MainCamera)
			return;

		Eclipse::Graphics::IGraphicsBuffer* buffer = Graphics::RendererManager::GetRenderer().GetGraphicsBuffer();

		float aspectRatio = WindowSize.y / WindowSize.x;
		Camera::MainCamera->GetBuffer().resolutionRatio = aspectRatio;

		Camera::MainCamera->GetBuffer().cameraPosition = Camera::MainCamera->gameObject->transform->GetPosition();
		Camera::MainCamera->GetBuffer().cameraRotation = Camera::MainCamera->gameObject->transform->GetRotation();
		Camera::MainCamera->GetBuffer().cameraScale = { Camera::MainCamera->CameraZoom, Camera::MainCamera->CameraZoom };

		buffer->SetOrCreateBuffer(Graphics::GraphicsBufferEnum::g_CameraBuffer, 0, &Camera::MainCamera->GetBuffer());
	}

	void SceneRenderer::SetResolution(Math::Vector2f resolution)
	{
		WindowSize = resolution;

		Eclipse::Graphics::IGraphicsDevice* device = Graphics::RendererManager::GetRenderer().GetDevice();
		device->SetViewport(WindowSize);

		device->BindTexture(GameFrameBuffer.textureIndex);
		device->ChangeImageDimensions(WindowSize, true);

		device->BindTexture(LightsFrameBuffer.textureIndex);
		device->ChangeImageDimensions(WindowSize, true);

		device->BindTexture(ShadowsFrameBuffer.textureIndex);
		device->ChangeImageDimensions(WindowSize, true);

		device->BindTexture(0);
	}
}