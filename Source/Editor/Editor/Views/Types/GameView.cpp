#include "GameView.h"

#include "Input/Input.h"

#include "ImGui/imgui.h"

#include "Renderer/RenderCommands/CommandList.h"

#include "Core/MainSingleton.h"

#include "Renderer/RendererManager.h"
#include "Renderer/IRenderer.h"
#include "Renderer/IGraphicsBuffer.h"

#include "EclipsedEngine/Components/Rendering/Camera.h"

#include "EclipsedEngine/Components/Transform2D.h"

namespace Eclipse::Editor
{
	void GameView::OnOpen()
	{
		ActiveResolutionIndex = 1;
		PopulateDefaultResolutions();
		ActiveResolutionRatios = myResolutions[ActiveResolutionIndex].AspectRatioResolution;

		InitSceneBuffer();
		Input::Input::AddGameViewWindow(viewId);

		ActiveImGuiFlag = ImGuiWindowFlags_MenuBar;

		auto device = Graphics::RendererManager::GetRenderer().GetDevice();
		device->BindFrameBuffer(0);
		device->BindTexture(0);
	}

	void GameView::PopulateDefaultResolutions()
	{
		myResolutions.clear();

		myResolutions.emplace_back("Free Aspect", Math::Vector2ui(1, 1));


		myResolutions.emplace_back("16:9 Wide", Math::Vector2ui(1280, 720));
		myResolutions.emplace_back("16:10 Wide", Math::Vector2ui(1280, 800));

		myResolutions.emplace_back("21:9 Ultra Wide", Math::Vector2ui(2560, 1080));
		myResolutions.emplace_back("32:9 Super Ultra Wide", Math::Vector2ui(3840, 1080));

		myResolutions.emplace_back("4:3 Standard", Math::Vector2ui(1280, 960));
		myResolutions.emplace_back("1:1 Square", Math::Vector2ui(1, 1));

		myResolutions.emplace_back("9:16 Wide", Math::Vector2ui(720, 1280));
	}

	void GameView::Draw()
	{
		CheckNChangeSceneImageDimension();

		MenuBar();

		Eclipse::Graphics::IGraphicsDevice* device = Graphics::RendererManager::GetRenderer().GetDevice();

		device->BindFrameBuffer(myGameFrameBuffer.frameBufferIndex);
		//device->Clear(Graphics::ClearFlags::Color, ClearColor);

		device->SetViewport(myWindowSize);

		SetBuffers();

		bool NoCameraInScene = false;
		if (Camera* camera = MainSingleton::GetPointer<Camera>())
		{
			device->Clear(Graphics::ClearFlags::Color, camera->GetClearColor());

			Graphics::CommandListManager* commandListManager = MainSingleton::GetPointer<Graphics::CommandListManager>();
			commandListManager->GetSpriteCommandList().Execute();
			commandListManager->GetUICommandList().Execute();
			commandListManager->GetDebugDrawCommandList().Execute();
		}
		else
			NoCameraInScene = true;

		DrawFixedResolution();


		if (NoCameraInScene)
		{
			device->Clear(Graphics::ClearFlags::Color, { 0.05f, 0.05f, 0.05f, 1.f });

			ImVec2 Size = ImGui::GetWindowSize();
			ImVec2 HalfContentSize(Size.x * 0.5f, Size.y * 0.5f);

			ImVec2 TextSize = ImGui::CalcTextSize("No Camera Rendering");
			TextSize.x *= 0.5f;
			TextSize.y *= 0.5f;

			HalfContentSize.x -= TextSize.x;
			HalfContentSize.y -= TextSize.y - 22.f;

			ImGui::SetCursorPos(HalfContentSize);
			ImGui::Text("No Camera Rendering");
		}

		device->BindFrameBuffer(0);
	}

	void GameView::DrawFixedResolution()
	{
		ImVec2 ContentRegionAvail = ImGui::GetContentRegionAvail();

		if (ActiveResolutionIndex)
		{
			ImVec2 NewCursorPosition = ImGui::GetCursorPos();
			float YDimensonSize = ContentRegionAvail.x * ActiveResolutionRatios.y;
			if (ContentRegionAvail.y > YDimensonSize)
			{
				float WindowSizeYRest = ContentRegionAvail.y - YDimensonSize;
				float HalfWindowSizeYRest = WindowSizeYRest * 0.5f;

				NewCursorPosition.y += HalfWindowSizeYRest;
				ContentRegionAvail.y = YDimensonSize;
			}
			else
			{
				float XDimensonSize = ContentRegionAvail.y * ActiveResolutionRatios.x;

				float WindowSizeXRest = ContentRegionAvail.x - XDimensonSize;
				float HalfWindowSizeXRest = WindowSizeXRest * 0.5f;

				NewCursorPosition.x += HalfWindowSizeXRest;
				ContentRegionAvail.x = XDimensonSize;
			}
			ImGui::SetCursorPos(NewCursorPosition);
		}

		myWindowSize = { ContentRegionAvail.x, ContentRegionAvail.y };

		ImGui::Image(myGameFrameBuffer.frameBufferIndex, ContentRegionAvail, ImVec2(0, 1), ImVec2(1, 0));
	}

	void GameView::MenuBar()
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Ratio"))
			{
				for (int i = 0; i < myResolutions.size(); i++)
				{
					if (!ImGui::Selectable(myResolutions[i].Name.c_str()))
						continue;

					ActiveResolutionIndex = i;
					ActiveResolutionRatios = myResolutions[i].AspectRatioResolution;
				}

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}
	}

	void GameView::InitSceneBuffer()
	{
		auto device = Graphics::RendererManager::GetRenderer().GetDevice();
		myGameFrameBuffer = device->CreateFrameBuffer();
	}

	void GameView::SetBuffers()
	{
		Eclipse::Graphics::IGraphicsBuffer* buffer = Graphics::RendererManager::GetRenderer().GetGraphicsBuffer();

		Camera* camera = MainSingleton::GetPointer<Camera>();

		float aspectRatio = myWindowSize.y / myWindowSize.x;
		camera->myCameraBuffer.resolutionRatio = aspectRatio;

		camera->myCameraBuffer.cameraPosition = camera->gameObject->transform->GetPosition();
		camera->myCameraBuffer.cameraRotation = camera->gameObject->transform->GetRotation();
		camera->myCameraBuffer.cameraScale = { camera->CameraZoom, camera->CameraZoom };

		buffer->SetOrCreateBuffer<CameraBuffer>(0, camera->myCameraBuffer);
	}

	void GameView::CheckNChangeSceneImageDimension()
	{
		Eclipse::Graphics::IGraphicsDevice* device = Graphics::RendererManager::GetRenderer().GetDevice();
		if (myWindowSize.x != myLastWindowResolution.x || myWindowSize.y != myLastWindowResolution.y)
		{
			device->BindTexture(myGameFrameBuffer.textureIndex);
			device->ChangeImageDimensions(myWindowSize);
			device->BindTexture(0);
		}

		myLastWindowResolution = { myWindowSize.x, myWindowSize.y };
	}
}