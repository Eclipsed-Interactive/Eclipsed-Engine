#ifdef ECL_EDITOR
#include "SceneView.h"
#include "HierarchyView.h"

#include "ImGui/imgui.h"

#include "Renderer/RenderCommands/CommandList.h"

#include "Input/Input.h"

#include "EclipsedEngine/Reflection/Registry/ComponentRegistry.h"

#include "EclipsedEngine/Components/Rendering/SpriteRenderer2D.h"

#include "EclipsedEngine/Components/Transform2D.h"

#include "EclipsedEngine/Components/UI/RectTransform.h"

#include "Editor/SelectionContext.h"
#include "Editor/Common/EditorActions.h"

#include "Core/Math/CommonMath.h"

#include "EclipsedEngine/Components/UI/Canvas.h"
#include "EclipsedEngine/Components/Rendering/Camera.h"

#include "Core/Clipboard.h"

#include "EclipsedEngine/ECS/ObjectManager.h"

#include <filesystem>
#include "Core/GraphicsBuffers/CameraBuffer.h"

#include "Core/GraphicsBuffers/EditorBuffer.h"
#include "Renderer/RendererManager.h"
#include "Renderer/IRenderer.h"
#include "Renderer/IGraphicsBuffer.h"

#include "Core/PathManager.h"

#include <ImGui/imgui_internal.h>

void Eclipse::Editor::SceneView::ZoomToObject(unsigned aObject)
{
	Transform2D* transform = ComponentManager::GetComponent<Transform2D>(aObject);
	if (transform)
	{
		myInspectorPosition = transform->GetPosition();
		myInspectorScale = Math::Vector2f(1, 1);
		totalYScroll = 0;
		return;
	}

	RectTransform* rectTransform = ComponentManager::GetComponent<RectTransform>(aObject);
	if (rectTransform)
	{
		myInspectorPosition = rectTransform->myCanvas->gameObject->transform->GetPosition();
		myInspectorScale = Math::Vector2f(1, 1);
		totalYScroll = 0;
		return;
	}
}

void Eclipse::Editor::SceneView::ResetCamera()
{
	myInspectorPosition = { 0, 0 };
	myInspectorScale = Math::Vector2f(1, 1);
	totalYScroll = 0;
}

void Eclipse::Editor::SceneView::ScrollManager()
{
	ImGuiIO& io = ImGui::GetIO();
	float deltaYScroll = io.MouseWheel;

	if (deltaYScroll == 0.0f)
		return;

	float baseFactor = 1.07f;

	float scaleMagnitude = std::log2(myInspectorScale.x + 1.0f);
	float dynamicFactor = baseFactor - (0.02f * scaleMagnitude);

	dynamicFactor = Math::Max(1.001f, dynamicFactor);
	float zoomFactor = (deltaYScroll > 0) ? dynamicFactor : 1.0f / dynamicFactor;

	Math::Vector2f zoomCenter = normalizedMousePosition + myInspectorPosition;

	myInspectorPosition = zoomCenter + (myInspectorPosition - zoomCenter) * (1.0f / zoomFactor);

	myInspectorScale *= {zoomFactor, zoomFactor};
}

void Eclipse::Editor::SceneView::MouseManager()
{
	{
		ImVec2 mousePos = ImGui::GetMousePos();
		ImVec2 cursorScreenPos = ImGui::GetCursorScreenPos();

		// cursorScreenPos.x -= 8;
		// cursorScreenPos.y -= 8;

		float mousePosX = mousePos.x - cursorScreenPos.x;
		float mousePosY = myWindowSize.y - (mousePos.y - cursorScreenPos.y);

		windowRelativeMousePosition = { static_cast<unsigned>(mousePosX), static_cast<unsigned>(mousePosY) };

		normalizedMousePosition.x = windowRelativeMousePosition.x / myWindowSize.x;
		normalizedMousePosition.y = windowRelativeMousePosition.y / myWindowSize.y;

		normalizedMousePosition.x = normalizedMousePosition.x * 2.f - 1;
		normalizedMousePosition.y = normalizedMousePosition.y * 2.f - 1;
	}

	if (ImGui::IsWindowHovered())
	{
		if (ImGui::IsMouseDown(1) || ImGui::IsMouseDown(2))
			mouseIsDown = true;

		ScrollManager();
	}

	if (!(ImGui::IsMouseDown(1) || ImGui::IsMouseDown(2)))
		mouseIsDown = false;

	if (mouseIsDown)
	{
		ImGuiIO& io = ImGui::GetIO();
		ImVec2 mouseDelta = io.MouseDelta;
		float ResolutionRatio = myWindowSize.x / myWindowSize.y;
		Math::Vector2f Dividend = myWindowSize * 0.5f;
		Math::Vector2f mouseDragdelta = { mouseDelta.x, mouseDelta.y };
		myInspectorPosition -= Math::Vector2f(mouseDragdelta.x / Dividend.y, -mouseDragdelta.y / (Dividend.x / ResolutionRatio)) * (1.f / myInspectorScale);

		//GraphicsEngine::Get()->SetCursor(GraphicsEngine::MouseCursor::Grab);
	}

	SpriteDragging();
}

void Eclipse::Editor::SceneView::SpriteDragging()
{
	if (!draggingSprite)
		return;

	ImGuiIO& io = ImGui::GetIO();
	ImVec2 mouseDelta = io.MouseDelta;
	float ResolutionRatio = myWindowSize.x / myWindowSize.y;
	Math::Vector2f Dividend = myWindowSize * 0.5f;
	Math::Vector2f mouseDragdelta = { mouseDelta.x, mouseDelta.y };
	Math::Vector2f mouseDeltaECL = Math::Vector2f(mouseDragdelta.x / Dividend.y, -mouseDragdelta.y / (Dividend.x / ResolutionRatio)) * (1.f / myInspectorScale);

	unsigned currentGO = SelectionContext::GetCurrentData<GameObjectTarget>();

	mySpriteMoveVector += mouseDeltaECL;

	Math::Vector2f positionSnappPosition = mySpriteMoveVector;
	if (myIsSnapping)
	{
		float roundX = std::round(mySpriteMoveVector.x / mySnappingDistance) * mySnappingDistance;
		float roundY = std::round(mySpriteMoveVector.y / mySnappingDistance) * mySnappingDistance;
		positionSnappPosition = { roundX, roundY };
	}

	Transform2D* transform = ComponentManager::GetComponent<Transform2D>(currentGO);
	if (transform)
	{
		transform->SetPosition(mySpriteMouseDownPosition + positionSnappPosition);
		transform->DirtyUpdate();
	}

	RectTransform* rectTransform = ComponentManager::GetComponent<RectTransform>(currentGO);
	if (rectTransform)
	{
		Math::Vector2f translatedPosition = Math::Vector2f(rectTransform->myCanvas->ReferenceResolution->y, rectTransform->myCanvas->ReferenceResolution->y);
		rectTransform->SetPosition(mySpriteMouseDownPosition + positionSnappPosition * translatedPosition * 0.5f);
	}

	if (ImGui::IsMouseReleased(0))
		draggingSprite = false;
}

Eclipse::Math::Vector2ui Eclipse::Editor::SceneView::GetSceneViewMousePosition()
{
	ImVec2 mouse = ImGui::GetMousePos();

	ImVec2 contentMin = ImGui::GetWindowContentRegionMin();
	ImVec2 contentMax = ImGui::GetWindowContentRegionMax();
	ImVec2 contentSize = ImGui::GetContentRegionAvail();
	ImVec2 windowPos = ImGui::GetWindowPos();

	ImVec2 contentScreenMin;
	ImVec2 contentScreenMax;

	contentScreenMin.x = windowPos.x + contentMin.x;
	contentScreenMin.y = windowPos.y + contentMin.y;

	contentScreenMax.x = windowPos.x + contentMax.x;
	contentScreenMax.y = windowPos.y + contentMax.y;

	if (mouse.x >= contentScreenMin.x &&
		mouse.x <= contentScreenMax.x &&
		mouse.y >= contentScreenMin.y &&
		mouse.y <= contentScreenMax.y)
	{
		return Math::Vector2ui(
			static_cast<unsigned int>(mouse.x - contentScreenMin.x),
			static_cast<unsigned int>(mouse.y - contentScreenMin.y)
		);
	}

	return {};
}



void Eclipse::Editor::SceneView::SpriteSelector()
{
	if (!ImGui::IsMouseClicked(0))
		return;
	if (!ImGui::IsWindowHovered())
		return;
	if (myGizmoMoveY || myGizmoMoveX)
		return;


	auto device = Graphics::RendererManager::GetRenderer().GetDevice();
	device->BindFrameBuffer(sceneBuffer.frameBufferIndex);
	device->Clear();
	auto buffer = Graphics::RendererManager::GetRenderer().GetGraphicsBuffer();
	
	//GraphicsEngine::Get<OpenGLGraphicsEngine>()->ClearCurrentSceneBuffer(0, 0, 0);

	//BaseGraphicsBuffer* graphicsBuffer = GraphicsEngine::Get()->GetGraphicsBuffer();

	EditorBuffer* editorBuffer;
	buffer->GetBuffer<EditorBuffer>(editorBuffer);
	editorBuffer->notOverideColor = 0;
	buffer->SetOrCreateBuffer<EditorBuffer>(35);

	BaseRenderComponent::IsScene = true;
	Canvas::IsScene = true;

	buffer->SetOrCreateBuffer<CameraBuffer>(0);

	CanvasBuffer* canvasBuffer;
	buffer->GetBuffer<CanvasBuffer>(canvasBuffer);
	canvasBuffer->canvasPositionOffset = Math::Vector2f(0, 0);


	Graphics::CommandListManager* commandListManager = MainSingleton::GetPointer<Graphics::CommandListManager>();
	commandListManager->GetSpriteCommandList().Execute();
	commandListManager->GetUICommandList().Execute();


	

	Math::Vector2i MousePosition = GetSceneViewMousePosition();

	//MousePosition.x = ImGui::GetContentRegionAvail().x - MousePosition.x;
	MousePosition.y = ImGui::GetContentRegionAvail().y - MousePosition.y;

	Math::Vector4ui colorValue = device->ReadPixelOnFrameBuffer(0, MousePosition);
	unsigned pickedID = colorValue.x + colorValue.y * 256 + colorValue.z * 256 * 256;

	unsigned currentGO = SelectionContext::GetCurrentData<GameObjectTarget>();
	if (currentGO == pickedID && currentGO)
	{
		if (ImGui::IsKeyDown(ImGuiKey_LeftAlt) || ImGui::IsKeyDown(ImGuiKey_RightAlt))
		{
			EditorActions::CopyObject(currentGO, true);

			char* data = (char*)ClipBoard::GetClipboardData();
			GameObject* gameobject = InternalSpawnObjectClass::CreateObjectFromJsonString(data);
			SelectionContext::SetActiveContext(GameObjectTarget{ gameobject->GetID() });


			GameObject* pickedGameobject = ComponentManager::GetGameObject(pickedID);


			gameobject->SetParent(nullptr);
		}

		Transform2D* transform = ComponentManager::GetComponent<Transform2D>(SelectionContext::GetCurrentData<GameObjectTarget>());
		if (transform)
		{
			draggingSprite = true;

			mySpriteMouseDownPosition = transform->GetLocalPosition();
			mySpriteMoveVector = { 0, 0 };
		}

		RectTransform* rectTransform = ComponentManager::GetComponent<RectTransform>(SelectionContext::GetCurrentData<GameObjectTarget>());
		if (rectTransform)
		{
			draggingSprite = true;

			mySpriteMouseDownPosition = rectTransform->GetLocalPosition();
			mySpriteMoveVector = { 0, 0 };
		}

		//GameObject* gm = transform->gameObject->GetTopParent();

		//pickedID = gm->GetID();
		//SelectionContext::SetActiveContext(GameObjectTarget{ pickedID });
		//HierarchyView::CurrentGameObjectID = pickedID;

		//if (pickedID)
		//	HierarchyView::OpenParents(pickedID);
	}
	else
	{
		SelectionContext::SetActiveContext(GameObjectTarget{ pickedID });
		HierarchyView::CurrentGameObjectID = pickedID;

		if (pickedID)
			HierarchyView::OpenParents(pickedID);
	}
}


void Eclipse::Editor::SceneView::ObjectSnappingGizmo()
{
	ImGui::Checkbox("Snap##MoreSnappingIDS", &myIsSnapping);

	if (myIsSnapping)
	{
		ImGui::Dummy({ 30, 0 });
		ImGui::SetNextItemWidth(75);
		ImGui::DragFloat("SnappDistance##SnappingDistance", &mySnappingDistance, 0.01f);
	}

	ImGui::SetCursorPosX(0);
}

/*void Eclipse::Editor::SceneView::GizmoManager(::Eclipse::Transform2D* aTransform)
{
	Math::Vector2f transformPos = aTransform->GetPosition();
	Math::Vector2f position = aTransform->GetPosition() * 0.5f + Math::Vector2f(0.5f, 0.5f);

	if (myGizmoMoveY)
		DebugDrawer::DrawArrow(position, Math::Vector2f(0, 1.f), 0.2f, 0.04f, Math::Color(0.2f, 1, 0.2f, 1));
	else
		DebugDrawer::DrawArrow(position, Math::Vector2f(0, 1.f), 0.2f, 0.04f, Math::Color(0, 1, 0, 1));

	if (myGizmoMoveX)
		DebugDrawer::DrawArrow(position, Math::Vector2f(1.f, 0.f), 0.2f, 0.04f, Math::Color(1, 0.2f, 0.2f, 1));
	else
		DebugDrawer::DrawArrow(position, Math::Vector2f(1.f, 0.f), 0.2f, 0.04f, Math::Color(1, 0, 0, 1));

	if (ImGui::IsMouseClicked(0))
	{
		float transformedGOPositionX = position.x - myInspectorPosition.x;
		float transformedGOPositionY = position.y - myInspectorPosition.y;

		float aspectRatio = myWindowSize.y / myWindowSize.x;

		float normMousePosX = windowRelativeMousePosition.x / myWindowSize.x;
		float normMousePosY = windowRelativeMousePosition.y / myWindowSize.y;

		float transformedMousePositionX = normMousePosX;
		float transformedMousePositionY = normMousePosY;

		float mouseXDistance = transformedGOPositionX - transformedMousePositionX;
		float mouseYDistance = transformedGOPositionY - transformedMousePositionY;

		float absXPos = std::abs(mouseXDistance);
		float absYPos = std::abs(mouseYDistance);

		if (absXPos < 0.1f)
		{
			myGizmoMoveY = true;
		}
		if (absYPos < 0.1f)
		{
			myGizmoMoveX = true;
		}
	}
}*/

void Eclipse::Editor::SceneView::Draw()
{

	ImVec2 windowSize = ImGui::GetContentRegionAvail();
	myWindowSize = { windowSize.x, windowSize.y };

	if (ImGui::BeginMenuBar())
	{
		ObjectSnappingGizmo();
		DrawGizmoButtons(DrawGizmo);
		ImGui::EndMenuBar();
	}

	MouseManager();


	if (SelectionContext::GetCurrentData<GameObjectTarget>() && ImGui::IsKeyPressed(ImGuiKey_F, false) && !ImGui::IsAnyItemActive())
		ZoomToObject(SelectionContext::GetCurrentData<GameObjectTarget>());

	if (SelectionContext::GetCurrentData<GameObjectTarget>())
	{
		mySelectedObject = GetComp(::Eclipse::SpriteRenderer2D, SelectionContext::GetCurrentData<GameObjectTarget>());

		if (mySelectedObject && mySelectedObject->GetMaterial())
		{
			::Eclipse::Transform2D* transform = GetComp(::Eclipse::Transform2D, SelectionContext::GetCurrentData<GameObjectTarget>());

			//DebugDrawer::Get().Begin();
			Math::Vector2f textureScale = mySelectedObject->GetMaterial()->GetTexture().GetSizeNormalized();
			Math::Vector2f size = mySelectedObject->spriteRectMax - mySelectedObject->spriteRectMin;
			float aspectScale = size.y / size.x;

			//DebugDrawer::DrawSquare(transform->GetPosition() * 0.5f + Math::Vector2f(0.5f, 0.5f), transform->GetRotation(), transform->GetScale() * 0.005f * 0.5f * textureScale * Math::Vector2f(1.f, aspectScale), Math::Color(1.f, 0.4f, 0.7f, 1.f));

			//GizmoManager(transform);

			//DebugDrawer::Get().Render();
		}
	}


	auto device = Graphics::RendererManager::GetRenderer().GetDevice();
	auto buffer = Graphics::RendererManager::GetRenderer().GetGraphicsBuffer();

	CameraBuffer* cameraBuffer = nullptr;
	buffer->GetBuffer<CameraBuffer>(cameraBuffer);

	Math::Vector2f lastInspectorPosition = cameraBuffer->cameraPosition;
	float lastInspectorRotation = cameraBuffer->cameraRotation;
	Math::Vector2f lastInspectorScale = cameraBuffer->cameraScale;

	float aspectRatio = myWindowSize.y / myWindowSize.x;
	cameraBuffer->resolutionRatio = aspectRatio;

	cameraBuffer->cameraPosition = myInspectorPosition;
	cameraBuffer->cameraRotation = myInspectorRotation;
	cameraBuffer->cameraScale = myInspectorScale;


	device->SetViewport(myWindowSize);


	buffer->SetOrCreateBuffer<CameraBuffer>(0);

	CanvasBuffer* canvasBuffer;
	buffer->GetBuffer<CanvasBuffer>(canvasBuffer);
	canvasBuffer->canvasPositionOffset = Math::Vector2f(0, 0);
	BaseRenderComponent::IsScene = true;
	Canvas::IsScene = true;


	// This is not using its own framebuffer but if left click then render and get mouse position color
	SpriteSelector();

	EditorBuffer* editorBuffer;
	buffer->GetBuffer<EditorBuffer>(editorBuffer);
	editorBuffer->notOverideColor = 1;
	buffer->SetOrCreateBuffer<EditorBuffer>(35);

	device->BindFrameBuffer(sceneBuffer.frameBufferIndex);
	device->Clear(Graphics::ClearFlags::Color, ClearColor);

	// isScene = 0;
	// GraphicsEngine::Get<OpenGLGraphicsEngine>()->UpdateGlobalUniform(UniformType::Int, "IsSceneView", &isScene);

	if (myWindowSize.x != myLastWindowResolution.x || myWindowSize.y != myLastWindowResolution.y)
	{
		// REsize the texture.
		//device->BindTexture()

		device->BindTexture(sceneBuffer.textureIndex);
		device->ChangeImageDimensions(myWindowSize);
		device->BindTexture(0);
	}

	myLastWindowResolution = { myWindowSize.x, myWindowSize.y };

	Graphics::CommandListManager* commandListManager = MainSingleton::GetPointer<Graphics::CommandListManager>();
	commandListManager->GetSpriteCommandList().Execute();
	commandListManager->GetUICommandList().Execute();
	commandListManager->GetDebugDrawCommandList().Execute();


	cameraBuffer->cameraPosition = lastInspectorPosition;
	cameraBuffer->cameraRotation = lastInspectorRotation;
	cameraBuffer->cameraScale = lastInspectorScale;


	ImVec2 CursorPos = ImGui::GetCursorPos();
	ImGui::SetCursorPos(ImVec2(CursorPos.x, CursorPos.y));
	ImGui::Image(sceneBuffer.textureIndex, ImVec2(myWindowSize.x, myWindowSize.y), ImVec2(0, 1), ImVec2(1, 0));

	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_PREFAB"))
		{
			char* charString = reinterpret_cast<char*>(malloc(payload->DataSize + 1));
			memcpy(charString, payload->Data, payload->DataSize);
			memset(charString + payload->DataSize, '\0', 1);

			std::filesystem::path path = PathManager::GetAssetsPath() / charString;

			free(charString);

			std::ifstream stream(path);
			if (!stream.is_open())
			{
				return;
			}

			size_t prefSize = std::filesystem::file_size(path);

			char* data = reinterpret_cast<char*>(malloc(prefSize + 1));
			stream.read(data, prefSize);
			memset(data + prefSize, '\0', 1);
			stream.close();

			InternalSpawnObjectClass::CreateObjectFromJsonString(data);

			free(data);
		}
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_SCENE"))
		{
			// Scene loading
		}
		ImGui::EndDragDropTarget();
	}

	device->BindFrameBuffer(0);
}

void Eclipse::Editor::SceneView::InitSceneBuffer()
{
	auto device = Graphics::RendererManager::GetRenderer().GetDevice();
	sceneBuffer = device->CreateFrameBuffer();
}

void Eclipse::Editor::SceneView::InitSelectedObjectShader()
{
	// unsigned vertexShaderID = 0;
	// unsigned pixelShaderID = 0;

	//ResourcePointer<VertexShader> myVertexShader = Resources::Get<VertexShader>((PathManager::GetEngineAssetsPath() / "Default/Shaders/SelectedVertexSpriteShader.glsl").generic_string().c_str());
	//ResourcePointer<PixelShader> myPixelShader = Resources::Get<PixelShader>((PathManager::GetEngineAssetsPath() / "Default/Shaders/SelectedPixelSpriteShader.glsl").generic_string().c_str());

	// mySelectedSpriteHighlightProgram = glCreateProgram();
	// glAttachShader(mySelectedSpriteHighlightProgram, myVertexShader->GetVertexShaderID());
	// glAttachShader(mySelectedSpriteHighlightProgram, myPixelShader->GetPixelShaderID());
	// glLinkProgram(mySelectedSpriteHighlightProgram);
}

void Eclipse::Editor::SceneView::OnOpen()
{
	ActiveImGuiFlag = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;

	InitSceneBuffer();
	InitSelectedObjectShader();

	auto device = Graphics::RendererManager::GetRenderer().GetDevice();
	device->BindFrameBuffer(0);
	device->BindTexture(0);
}
#endif
