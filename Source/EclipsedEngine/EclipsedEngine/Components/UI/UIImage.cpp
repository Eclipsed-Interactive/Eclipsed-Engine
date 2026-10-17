#include "UIImage.h"

#include "Renderer/Sprite.h"

#include "Assets/AssetManager.h"
#include "Core/Settings/GraphicsSettings.h"

#include "EclipsedEngine/Components/UI/RectTransform.h"
#include "EclipsedEngine/Components/UI/Canvas.h"

#include "Renderer/RenderCommands/CommandList.h"

#ifdef ECL_EDITOR
//#include "Renderer/DebugDrawers/DebugDrawer.h"
#include "Core/GraphicsBuffers/EditorBuffer.h"
#endif // ECL_EDITOR

#include "Core/GraphicsBuffers/EditorBuffer.h"

#include "Renderer/IRenderer.h"
#include "Renderer/IDrawer.h"
#include "Renderer/RendererManager.h"

#include "Renderer/RenderCommands/CommandList.h"

namespace Eclipse
{
#ifdef ECLIPSED_NETWORKING
	void UIImage::sprite_OnRep()
	{
	}
#endif

#ifdef ECL_EDITOR
	void UIImage::EditorUpdate()
	{
		auto transform = gameObject->GetComponent<RectTransform>();

		if (transform->myCanvas)
		{
			float sizeX = (transform->WidthHeightPX->x / transform->myCanvas->ReferenceResolution->y) * 0.5f;
			float sizeY = (transform->WidthHeightPX->y / transform->myCanvas->ReferenceResolution->y) * 0.5f;

			float posX = transform->Position->x / transform->myCanvas->ReferenceResolution->y + 0.5f;
			float posY = transform->Position->y / transform->myCanvas->ReferenceResolution->y + 0.5f;

			Math::Vector2f sqrPos = { posX, posY };
			Math::Vector2f sqrSize = { sizeX, sizeY };

			//DebugDrawer::DrawSquare(sqrPos + transform->myCanvas->canvasCameraTransform.PositionOffset, 0, sqrSize, Math::Color(0x90D5FF));
		}
	}
#endif // ECL_EDITOR
	void UIImage::OnComponentAdded()
	{
		if (material->IsValid()) hasMaterial = true;
		if (sprite->IsValid()) hasSprite = true;

		if (!hasMaterial)
		{
			material = Assets::AssetManager::LoadDefault<Assets::Material>(Assets::DefaultAssetType::MATERIAL_UI);
			hasMaterial = true;
		}
	}

	void UIImage::Render()
	{
		Graphics::CommandListManager* commandListManager = MainSingleton::GetPointer<Graphics::CommandListManager>();

		commandListManager->GetUICommandList().Enqueue([&]()
			{
				Draw();
			});
	}

	void UIImage::TransformUpdate()
	{
		auto tranform = gameObject->GetComponent<RectTransform>();

		Canvas* canvas = tranform->myCanvas;
		if (!canvas)
		{
			tranform->myIsDirty = false;
			return;
		}

		Canvas::EditorCanvasCameraTransform& canvasCameraTransform = canvas->canvasCameraTransform;
		Math::Vector2f referenceResolution = canvas->ReferenceResolution;
		Math::Vector2f halfRefRes = referenceResolution * 0.5f;

#ifdef ECL_EDITOR
		Math::Vector2f resolution = Settings::GraphicsSettings::GetResolution();
#endif

		Math::Vector2f resMinRef = resolution - referenceResolution;

		Math::Vector2f halfRes = resolution * 0.5f;
		Math::Vector2f position = tranform->GetPosition();

		myTransformBuffer.Position = position;

		Math::Vector2f multiplier;

#ifdef ECL_EDITOR
		Graphics::IRenderer* Renderer = MainSingleton::GetPointer<Graphics::IRenderer>();
		Graphics::IGraphicsBuffer* GraphicsBuffer = Renderer->GetGraphicsBuffer();
		EditorBuffer* editorBuffer;
		GraphicsBuffer->GetBuffer<EditorBuffer>(editorBuffer);

		if (editorBuffer->IsSceneView)
		{
			myTransformBuffer.Position *= canvasCameraTransform.ScaleMultiplier;
			multiplier = canvasCameraTransform.ScaleMultiplier;
		}
		else
		{
			if (!tranform->ScaleWithCanvasX)
				multiplier.x = canvasCameraTransform.ScaleMultiplier.x;
			else
				multiplier.x = 2;
			if (!tranform->ScaleWithCanvasY)
				multiplier.y = canvasCameraTransform.ScaleMultiplier.y;
			else
				multiplier.y = 2;

			myTransformBuffer.Position *= Math::Vector2f(2, 2);
		}
#else
		myTransformBuffer.Position *= Math::Vector2f(2, 2);

		if (!tranform->ScaleWithCanvasX)
			multiplier.x = canvasCameraTransform.ScaleMultiplier.x;
		else
			multiplier.x = 2;
		if (!tranform->ScaleWithCanvasY)
			multiplier.y = canvasCameraTransform.ScaleMultiplier.y;
		else
			multiplier.y = 2;
#endif

		myTransformBuffer.Position += canvasCameraTransform.PositionOffset;

		Math::Vector2f WidthHeightPX = tranform->WidthHeightPX.Get();
		myTransformBuffer.Scale = WidthHeightPX;

		Math::Vector2f canvasScaleRelationOneDiv = { 1.f / referenceResolution.x, 1.f / referenceResolution.y };
		myTransformBuffer.Scale *= canvasScaleRelationOneDiv * multiplier;

		myTransformBuffer.Rotation = canvasCameraTransform.Rotation;
	}

	void UIImage::Draw()
	{

		if (!hasMaterial)
			return;


		auto transform = gameObject->GetComponent<RectTransform>();
		if (!transform)
			return;
		if (!transform->myCanvas)
			return;

		transform->myCanvas->SetCanvasTransformProperties();


		Math::Vector2f resolution = transform->myCanvas->ReferenceResolution;

		resolution.x = 1.f / resolution.x;
		resolution.y = 1.f / resolution.y;

		// float aspectRatio = resolution.y / resolution.x;
		// Math::Vector2f canvasScaleRelationOneDiv = {resolution.x, resolution.y};

		Math::Vector2f size = spriteRectMax - spriteRectMin;
		material->GetBuffer().spriteRect = {spriteRectMin.x, spriteRectMin.y, size.x, size.y};

		Graphics::IGraphicsDevice* graphicsDevice = Graphics::RendererManager::GetRenderer().GetDevice();
		graphicsDevice->BindMaterial(material);
		if (sprite->IsValid())
			graphicsDevice->BindTexture(0, sprite);

		TransformUpdate();

		Graphics::IGraphicsBuffer* buffer = Graphics::RendererManager::GetRenderer().GetGraphicsBuffer();

		CanvasBuffer* canvasBuffer;
		buffer->GetBuffer<CanvasBuffer>(canvasBuffer);

#ifdef ECL_EDITOR
		EditorBuffer* editorBuffer;
		buffer->GetBuffer<EditorBuffer>(editorBuffer);
		editorBuffer->PixelPickColor = gameObject->myPixelPickColor;
		buffer->SetOrCreateBuffer<EditorBuffer>(35);

		if (!editorBuffer->IsSceneView)
		{
			if (!transform->myCanvas->WorldSpace)
				canvasBuffer->canvasPositionOffset = transform->myCanvas->canvasCameraTransform.PositionOffset;
			else
				canvasBuffer->canvasPositionOffset = { 0, 0 };
		}
#else
		if (!transform->myCanvas->WorldSpace)
			canvasBuffer->canvasPositionOffset = transform->myCanvas->canvasCameraTransform.PositionOffset;
		else
			canvasBuffer->canvasPositionOffset = { 0, 0 };
#endif


		buffer->SetOrCreateBuffer(5, material->GetBuffer());
		buffer->SetOrCreateBuffer(1, myTransformBuffer);
		buffer->SetOrCreateBuffer(2, *canvasBuffer);

		Graphics::IRenderer* Renderer = MainSingleton::GetPointer<Graphics::IRenderer>();
		Graphics::IDrawer* Drawer = Renderer->GetDrawer();

		Drawer->DrawSprite();
	}
}
