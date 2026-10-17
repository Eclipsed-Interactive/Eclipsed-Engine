#include "TextRenderer.h"

#include "Renderer/Sprite.h"
#include "Renderer/IRenderer.h"
#include "Renderer/IDrawer.h"
#include "Renderer/RendererManager.h"
#include "Renderer/RenderCommands/CommandList.h"

#include "EclipsedEngine/Components/UI/RectTransform.h"
#include "EclipsedEngine/Components/UI/Canvas.h"

#include "Assets/Core/AssetDatabase.h"
#include "Assets/AssetManager.h"
#include "Core/MainSingleton.h"
#include "Core/GraphicsBuffers/TextMaterialBuffer.h"
#include "Core/GraphicsBuffers/TransformBuffer.h"

#include "Core/UtilityMacros.h"

#ifdef ECL_EDITOR
#include <Core/GraphicsBuffers/EditorBuffer.h>
#endif

namespace Eclipse
{
#ifdef ECL_EDITOR
	void TextRenderer::EditorUpdate()
	{
		// TODO: should be called form Comp manager
		OnDrawGizmos();

		if (myLastFontSize != myFontSize)
		{
			myLastFontSize = myFontSize;
		}
	}
#endif

	void TextRenderer::TransformUpdate()
	{
		auto tranform = gameObject->GetComponent<RectTransform>();

		Canvas* canvas = tranform->myCanvas;
		if (!canvas)
		{
			tranform->myIsDirty = false;
			return;
		}

		Canvas::EditorCanvasCameraTransform& canvasCameraTransform = canvas->canvasCameraTransform;
		Math::Vector2f resolution = canvas->ReferenceResolution;
		Math::Vector2f canvasScaleRelationOneDiv = { 1.f / resolution.x, 1.f / resolution.y };

		myTransformBuffer.Position = tranform->GetPosition();

#ifdef ECL_EDITOR
		Graphics::IRenderer* Renderer = MainSingleton::GetPointer<Graphics::IRenderer>();
		Graphics::IGraphicsBuffer* GraphicsBuffer = Renderer->GetGraphicsBuffer();
		EditorBuffer* editorBuffer;
		GraphicsBuffer->GetBuffer<EditorBuffer>(editorBuffer);

#endif
		myTransformBuffer.Position *= canvasCameraTransform.ScaleMultiplier;
		myTransformBuffer.Position += canvasCameraTransform.PositionOffset;

		myTransformBuffer.Scale = tranform->WidthHeightPX.Get() * canvasScaleRelationOneDiv * ((float)myFontSize * 0.005f);

		Math::Vector2f multiplier;


		if (!tranform->ScaleWithCanvasX)
			multiplier.x = canvasCameraTransform.ScaleMultiplier.x;
		else
			multiplier.x = 2;
		if (!tranform->ScaleWithCanvasY)
			multiplier.y = canvasCameraTransform.ScaleMultiplier.y;
		else
			multiplier.y = 2;

		myTransformBuffer.Scale *= multiplier;


		myTransformBuffer.Scale.x *= 2.f;

		myTransformBuffer.Rotation = canvasCameraTransform.Rotation;
	}


	void TextRenderer::OnComponentAdded()
	{
		material = Assets::AssetManager::LoadDefault<Assets::Material>(Assets::DefaultAssetType::MATERIAL_TEXT);

		gameObject->GetComponent<RectTransform>()->AddFunctionToRunOnDirtyUpdate([&]() {
			TransformUpdate();
			});

		font = Assets::AssetManager::LoadDefault<Assets::Font>(Assets::DefaultAssetType::TEXT_FONT);
	}

#ifdef ECL_EDITOR
	void TextRenderer::OnDrawGizmos()
	{
		//if (drawRectGizmos)
		//{
		//	auto tranform = gameObject->GetComponent<RectTransform>();
		//	if (!tranform)
		//		return;
		//	if (!tranform->myCanvas)
		//		return;

		//	Math::Vector2f resolution = tranform->myCanvas->ReferenceResolution;
		//	float aspectRatio = resolution.x / resolution.y;
		//	Math::Vector2f canvasScaleRelationOneDiv = { 1.f / resolution.x, 1.f / resolution.y };

		//	const Math::Vector2f& position = tranform->GetPosition() * canvasScaleRelationOneDiv * Math::Vector2f(aspectRatio, 1);
		//	//float rotation = gameObject->transform->GetRotation();
		//	Math::Vector2f scale = tranform->WidthHeightPX.Get() * myRect * canvasScaleRelationOneDiv;
		//	scale.x *= aspectRatio;


		//	//Math::Vector2f CanvasPosition = canvasScaleRelationOneDiv * tranform->myCanvas->canvasCameraTransform.PositionOffset * 0.5f;

		//	DebugDrawer::DrawSquareMinMax(position - scale, position + scale, Math::Color(0.7f, 0.7f, 0.7f, 1.f));
		//}
	}
#endif

	void TextRenderer::Render()
	{
		Graphics::CommandListManager* commandListManager = MainSingleton::GetPointer<Graphics::CommandListManager>();

		Eclipse::Graphics::CommandList* ActiveCommandlist = &commandListManager->GetUICommandList();

		auto transform = gameObject->GetComponent<RectTransform>();
		if (transform->myCanvas && transform->myCanvas->WorldSpace)
			ActiveCommandlist = &commandListManager->GetUICommandList();

		ActiveCommandlist->Enqueue([&]() {
			this->Draw();
			});
	}

	void TextRenderer::Draw()
	{
		if (!material.IsValid())
			return;
		if (!font.IsValid())
			return;

		auto transform = gameObject->GetComponent<RectTransform>();

		if (!transform)
			return;
		if (!transform->myCanvas)
			return;

		Graphics::IRenderer* Renderer = MainSingleton::GetPointer<Graphics::IRenderer>();
		Graphics::IGraphicsBuffer* GraphicsBuffer = Renderer->GetGraphicsBuffer();
		Graphics::IGraphicsDevice* Device = Renderer->GetDevice();

		transform->myCanvas->SetCanvasTransformProperties();

		Device->BindMaterial(material);


		Assets::InternalFont& CurrentFont = font->dataPtr->font;

		TransformUpdate();

		Math::Vector2f resolution = transform->myCanvas->ReferenceResolution;
		Math::Vector2f scaleRect = myTransformBuffer.Scale * myRect * resolution;
		scaleRect.x *= 0.5f;

		lineOffsets.resize(1);
		lineOffsets.back() = 0;

		const char* textInConstChar = myText->c_str();
		for (int i = 0; i < myText->size(); i++)
		{
			char character = textInConstChar[i];
			if (character == '\n')
			{
				lineOffsets.emplace_back(0.f);
				continue;
			}

			const Assets::Character& characterFace = CurrentFont.GetCharTexture(character);
			float convertedAdvance = (float)(characterFace.advance) * 100.f * myTransformBuffer.Scale.x;
			lineOffsets.back() += convertedAdvance * 0.5f * myTextAlignment * myCharacterSpacing;
		}


		Math::Vector2f textOffset = { 0, 0 };
		if (myTextAlignment == 0 || myTextAlignment == 3)
			textOffset.x -= scaleRect.x;
		else if (myTextAlignment == 2)
			textOffset.x += scaleRect.x - myTransformBuffer.Scale.x;

		Math::Vector2f startOffset = textOffset;
		if (myTextCentering == 0)
			;//textOffset.y = (scaleRect.y - myFontSize * 4.f);
		else if (myTextCentering == 1)
			;//textOffset.y = myFontSize * -3.f * lineOffsets.size();
		else if (myTextCentering == 2)
			;//textOffset.y = (-scaleRect.y + 0.0125f)  * lineOffsets.size();



		float LastCharacterAdvance = 0;
		int currentLineCount = 0;
		for (int i = 0; i < myText->size(); i++)
		{
			char character = textInConstChar[i];

			if (character == '\n')
			{
				textOffset.y -= myTransformBuffer.Scale.y * 200.f * myEnterSpacing * 2.f * myFontSize;
				textOffset.x = startOffset.x;
				LastCharacterAdvance = 0.f;

				currentLineCount++;

				continue;
			}

			if (!CurrentFont.CharacterExists(character))
				character = '\n';

			const Assets::Character& characterFace = CurrentFont.GetCharTexture(character);
			float characterAdvance = LastCharacterAdvance * 100.f * myTransformBuffer.Scale.x;


			Device->BindTexture(0, characterFace.textureID);

			myTextBuffer.size = { (float)characterFace.size.x, (float)characterFace.size.y };

			float lineOffset = 0;
			if (myTextAlignment != 0)
				lineOffset = lineOffsets[currentLineCount] - myTransformBuffer.Scale.x * myCharacterSpacing;

			textOffset.x += characterAdvance * myCharacterSpacing;

			LastCharacterAdvance = (float)(characterFace.advance);


			const float pxX = 100.f * myTransformBuffer.Scale.x;
			const float pxY = 100.f * myTransformBuffer.Scale.y;

			myTextBuffer.offset.x = textOffset.x + (characterFace.bearing.x + characterFace.size.x * 0.5f) * pxX;
			myTextBuffer.offset.y = textOffset.y + (characterFace.bearing.y - characterFace.size.y * 0.5f) * pxY;

			CanvasBuffer* canvasBuffer;
			GraphicsBuffer->GetBuffer<CanvasBuffer>(canvasBuffer);

#ifdef ECL_EDITOR
			EditorBuffer* editorBuffer;
			GraphicsBuffer->GetBuffer<EditorBuffer>(editorBuffer);
			editorBuffer->PixelPickColor = gameObject->myPixelPickColor;
			GraphicsBuffer->SetOrCreateBuffer<EditorBuffer>(35);

			if (editorBuffer->PixelPicking)
			{
				material.GetBuffer().color = { gameObject->myPixelPickColor.x, gameObject->myPixelPickColor .y, gameObject->myPixelPickColor .z, gameObject->myPixelPickColor.w };
			}
			else
				material.GetBuffer().color = myTextColor;

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

			graphicsDevice->BindMaterial(material);
#endif
			GraphicsBuffer->SetOrCreateBuffer(1, myTransformBuffer);
			GraphicsBuffer->SetOrCreateBuffer(2, *canvasBuffer);
			GraphicsBuffer->SetOrCreateBuffer(3, myTextBuffer);
			GraphicsBuffer->SetOrCreateBuffer(5, material.GetBuffer());

			Graphics::IRenderer& renderer = MainSingleton::GetInstance<Graphics::IRenderer>();
			Graphics::IDrawer* drawer = renderer.GetDrawer();
			drawer->DrawSprite();
		}
	}

	void TextRenderer::SetFontSize(float aFontSize)
	{
		myFontSize = aFontSize;
	}

	void TextRenderer::SetFont(const char* aFont)
	{

	}

	void TextRenderer::SetText(const char* aText)
	{
		myText = aText;
	}
}
