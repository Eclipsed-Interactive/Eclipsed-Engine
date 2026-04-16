#include "TextRenderer.h"

#include "GraphicsEngine/RenderCommands/CommandList.h"

#include "AssetEngine/Resources.h"
#include "GraphicsEngine/TextSprite.h"
#include "GraphicsEngine/OpenGL/OpenGLGraphicsAPI.h"

#include "EclipsedEngine/Components/UI/RectTransform.h"
#include "EclipsedEngine/Components/UI/Canvas.h"

#include "OpenGL/glad/glad.h"

#include "AssetEngine/Editor/MetaFile/MetaFileRegistry.h"
#include "CoreEngine/GraphicsBuffers/TextMaterialBuffer.h"
#include "CoreEngine/GraphicsBuffers/TransformBuffer.h"

namespace Eclipse
{
	TextMaterial::TextMaterial()
	{
		std::string VtxShaderGUID = MetaFileRegistry::GetGUIDMeta((PathManager::GetEngineAssetsPath() / "Default/Shaders/Text.pglsl.meta").generic_string());
		std::string PxShaderGUID = MetaFileRegistry::GetGUIDMeta((PathManager::GetEngineAssetsPath() / "Default/Shaders/Text.vglsl.meta").generic_string());

		pixelShader = Resources::Get<PixelShader>(VtxShaderGUID);
		vertexShader = Resources::Get<VertexShader>(PxShaderGUID);

		programID = glCreateProgram();
		glAttachShader(programID, vertexShader.GetProgramID());
		glAttachShader(programID, pixelShader.GetProgramID());
		glLinkProgram(programID);
	}

	void TextMaterial::Use(unsigned textureID)
	{
		glActiveTexture(GL_TEXTURE0);
		GraphicsEngine::Get<OpenGLGraphicsEngine>()->BindTexture(GL_TEXTURE_2D, textureID);
	}





	void TextRenderer::EditorUpdate()
	{
		// TODO: should be called form Comp manager
		OnDrawGizmos();

		if (myLastFontSize != myFontSize)
		{
			myLastFontSize = myFontSize;
		}

	}

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
		if (IsScene)
			myTransformBuffer.Position *= canvasCameraTransform.ScaleMultiplier;
		else
			myTransformBuffer.Position *= Math::Vector2f(2, 2);
		myTransformBuffer.Position += canvasCameraTransform.PositionOffset;
		
		myTransformBuffer.Scale = tranform->WidthHeightPX.Get() * canvasScaleRelationOneDiv * ((float)myFontSize * 0.005f);
		// if (!IsScene)
		// 	myTransformBuffer.Scale *= { 2.f, 2.f };
		
		Math::Vector2f multiplier;
		if (IsScene)
			multiplier = canvasCameraTransform.ScaleMultiplier;
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
		}
		myTransformBuffer.Scale *= multiplier;

		
		myTransformBuffer.Scale.x *= 2.f;
		
		myTransformBuffer.Rotation = canvasCameraTransform.Rotation;
	}


	void TextRenderer::OnComponentAdded()
	{
		myMaterial = new TextMaterial();

		gameObject->GetComponent<RectTransform>()->AddFunctionToRunOnDirtyUpdate([&](){
			TransformUpdate();
		});
	}

	//void TextRenderer::DrawInspector()
	//{
	//	ImGui::PushID(myInstanceComponentID);

	//	ImGui::Text("Text Color");
	//	ImGui::SameLine();
	//	if (ImGui::ColorButton("##TextColorpickerButton", ImVec4(myTextColor->r, myTextColor->g, myTextColor->b, myTextColor->a)))
	//		ImGui::OpenPopup("TextColorPickerPopup");

	//	if (ImGui::BeginPopup("TextColorPickerPopup"))
	//	{
	//		ImGui::ColorPicker4("##TextColorPicker", myTextColor->data);
	//		ImGui::EndPopup();
	//	}


	//	ImColor defaultcolor = ImGui::GetColorU32(ImGuiCol_Button, 1.f);

	//	ImColor onColor;
	//	onColor.Value.x = defaultcolor.Value.x * 0.7f;
	//	onColor.Value.y = defaultcolor.Value.y * 0.7f;
	//	onColor.Value.z = defaultcolor.Value.z * 0.7f;
	//	onColor.Value.w = 1.f;

	//	ImColor offColor;
	//	offColor.Value.x = defaultcolor.Value.x * 1.5f;
	//	offColor.Value.y = defaultcolor.Value.y * 1.5f;
	//	offColor.Value.z = defaultcolor.Value.z * 1.5f;
	//	offColor.Value.w = 1.f;

	//	DrawMultilineInspector();
	//	DrawAlignmentInspector(onColor, offColor);
	//	DrawCenteringInspector(onColor, offColor);

	//	ImGui::PopID();
	//}

	//void TextRenderer::DrawMultilineInspector()
	//{
	//	char textComponentText[8196];

	//	strcpy(textComponentText, myText->data());

	//	ImGui::Text("Text");
	//	if (ImGui::InputTextMultiline("##TextComponentText", textComponentText, 8196, ImVec2(0, 0)))
	//		myText = textComponentText;
	//}

	//void TextRenderer::DrawAlignmentInspector(const ImColor& onColor, const ImColor& offColor)
	//{
	//	ImColor color;

	//	if (myTextAlignment == 0)
	//		color = onColor;
	//	else
	//		color = offColor;

	//	ImVec2 leftCursorStart = ImGui::GetCursorScreenPos();
	//	ImGui::PushStyleColor(ImGuiCol_Button, color.Value);
	//	if (ImGui::Button("##ButtonForUseWithAlightLeft", ImVec2(40, 25)))
	//		myTextAlignment = 0;
	//	ImGui::PopStyleColor();

	//	ImVec2 leftCursorStart1 = ImVec2(leftCursorStart.x + 5.f, leftCursorStart.y + 5.f);
	//	ImVec2 leftCursorEnd1 = ImVec2(leftCursorStart.x + 30.f, leftCursorStart.y + 5.f);
	//	ImGui::GetWindowDrawList()->AddLine(leftCursorStart1, leftCursorEnd1, IM_COL32(200, 200, 200, 255));

	//	ImVec2 leftCursorStart2 = ImVec2(leftCursorStart.x + 5.f, leftCursorStart.y + 10.f);
	//	ImVec2 leftCursorEnd2 = ImVec2(leftCursorStart.x + 24.f, leftCursorStart.y + 10.f);
	//	ImGui::GetWindowDrawList()->AddLine(leftCursorStart2, leftCursorEnd2, IM_COL32(200, 200, 200, 255));

	//	ImVec2 leftCursorStart3 = ImVec2(leftCursorStart.x + 5.f, leftCursorStart.y + 15.f);
	//	ImVec2 leftCursorEnd3 = ImVec2(leftCursorStart.x + 27.f, leftCursorStart.y + 15.f);
	//	ImGui::GetWindowDrawList()->AddLine(leftCursorStart3, leftCursorEnd3, IM_COL32(200, 200, 200, 255));

	//	ImVec2 leftCursorStart4 = ImVec2(leftCursorStart.x + 5.f, leftCursorStart.y + 20.f);
	//	ImVec2 leftCursorEnd4 = ImVec2(leftCursorStart.x + 22.f, leftCursorStart.y + 20.f);
	//	ImGui::GetWindowDrawList()->AddLine(leftCursorStart4, leftCursorEnd4, IM_COL32(200, 200, 200, 255));

	//	ImGui::SameLine();

	//	if (myTextAlignment == 1)
	//		color = onColor;
	//	else
	//		color = offColor;

	//	ImVec2 middleCursorStart = ImGui::GetCursorScreenPos();
	//	ImGui::PushStyleColor(ImGuiCol_Button, color.Value);
	//	if (ImGui::Button("##ButtonForUseWithAlightMiddle", ImVec2(40, 25)))
	//		myTextAlignment = 1;
	//	ImGui::PopStyleColor();

	//	ImVec2 middleCursorStart1 = ImVec2(middleCursorStart.x + 7.5f, middleCursorStart.y + 5.f);
	//	ImVec2 middleCursorEnd1 = ImVec2(middleCursorStart.x + 32.5f, middleCursorStart.y + 5.f);
	//	ImGui::GetWindowDrawList()->AddLine(middleCursorStart1, middleCursorEnd1, IM_COL32(200, 200, 200, 255));

	//	ImVec2 middleCursorStart2 = ImVec2(middleCursorStart.x + 10.5f, middleCursorStart.y + 10.f);
	//	ImVec2 middleCursorEnd2 = ImVec2(middleCursorStart.x + 29.5f, middleCursorStart.y + 10.f);
	//	ImGui::GetWindowDrawList()->AddLine(middleCursorStart2, middleCursorEnd2, IM_COL32(200, 200, 200, 255));

	//	ImVec2 middleCursorStart3 = ImVec2(middleCursorStart.x + 7.f, middleCursorStart.y + 15.f);
	//	ImVec2 middleCursorEnd3 = ImVec2(middleCursorStart.x + 33.f, middleCursorStart.y + 15.f);
	//	ImGui::GetWindowDrawList()->AddLine(middleCursorStart3, middleCursorEnd3, IM_COL32(200, 200, 200, 255));

	//	ImVec2 middleCursorStart4 = ImVec2(middleCursorStart.x + 11.5f, middleCursorStart.y + 20.f);
	//	ImVec2 middleCursorEnd4 = ImVec2(middleCursorStart.x + 28.5f, middleCursorStart.y + 20.f);
	//	ImGui::GetWindowDrawList()->AddLine(middleCursorStart4, middleCursorEnd4, IM_COL32(200, 200, 200, 255));

	//	ImGui::SameLine();

	//	if (myTextAlignment == 2)
	//		color = onColor;
	//	else
	//		color = offColor;

	//	ImVec2 rightCursorStart = ImGui::GetCursorScreenPos();
	//	ImGui::PushStyleColor(ImGuiCol_Button, color.Value);
	//	if (ImGui::Button("##ButtonForUseWithAlightRight", ImVec2(40, 25)))
	//		myTextAlignment = 2;
	//	ImGui::PopStyleColor();

	//	ImVec2 rightCursorStart1 = ImVec2(rightCursorStart.x + 10.f, rightCursorStart.y + 5.f);
	//	ImVec2 rightCursorEnd1 = ImVec2(rightCursorStart.x + 35.f, rightCursorStart.y + 5.f);
	//	ImGui::GetWindowDrawList()->AddLine(rightCursorStart1, rightCursorEnd1, IM_COL32(200, 200, 200, 255));

	//	ImVec2 rightCursorStart2 = ImVec2(rightCursorStart.x + 18.f, rightCursorStart.y + 10.f);
	//	ImVec2 rightCursorEnd2 = ImVec2(rightCursorStart.x + 35.f, rightCursorStart.y + 10.f);
	//	ImGui::GetWindowDrawList()->AddLine(rightCursorStart2, rightCursorEnd2, IM_COL32(200, 200, 200, 255));

	//	ImVec2 rightCursorStart3 = ImVec2(rightCursorStart.x + 22.f, rightCursorStart.y + 15.f);
	//	ImVec2 rightCursorEnd3 = ImVec2(rightCursorStart.x + 35.f, rightCursorStart.y + 15.f);
	//	ImGui::GetWindowDrawList()->AddLine(rightCursorStart3, rightCursorEnd3, IM_COL32(200, 200, 200, 255));

	//	ImVec2 rightCursorStart4 = ImVec2(rightCursorStart.x + 19.f, rightCursorStart.y + 20.f);
	//	ImVec2 rightCursorEnd4 = ImVec2(rightCursorStart.x + 35.f, rightCursorStart.y + 20.f);
	//	ImGui::GetWindowDrawList()->AddLine(rightCursorStart4, rightCursorEnd4, IM_COL32(200, 200, 200, 255));

	//	ImGui::SameLine();

	//	if (myTextAlignment == 3)
	//		color = onColor;
	//	else
	//		color = offColor * 0.5f;

	//	ImVec2 filledCursorStart = ImGui::GetCursorScreenPos();
	//	ImGui::PushStyleColor(ImGuiCol_Button, color.Value);
	//	if (ImGui::Button("##ButtonForUseWithAlightfilled", ImVec2(40, 25)) && false)
	//		myTextAlignment = 3;
	//	ImGui::PopStyleColor();

	//	ImVec2 filledCursorStart1 = ImVec2(filledCursorStart.x + 5.f, filledCursorStart.y + 5.f);
	//	ImVec2 filledCursorEnd1 = ImVec2(filledCursorStart.x + 35.f, filledCursorStart.y + 5.f);
	//	ImGui::GetWindowDrawList()->AddLine(filledCursorStart1, filledCursorEnd1, IM_COL32(200, 200, 200, 255));

	//	ImVec2 filledCursorStart2 = ImVec2(filledCursorStart.x + 5.f, filledCursorStart.y + 10.f);
	//	ImVec2 filledCursorEnd2 = ImVec2(filledCursorStart.x + 35.f, filledCursorStart.y + 10.f);
	//	ImGui::GetWindowDrawList()->AddLine(filledCursorStart2, filledCursorEnd2, IM_COL32(200, 200, 200, 255));

	//	ImVec2 filledCursorStart3 = ImVec2(filledCursorStart.x + 5.f, filledCursorStart.y + 15.f);
	//	ImVec2 filledCursorEnd3 = ImVec2(filledCursorStart.x + 35.f, filledCursorStart.y + 15.f);
	//	ImGui::GetWindowDrawList()->AddLine(filledCursorStart3, filledCursorEnd3, IM_COL32(200, 200, 200, 255));

	//	ImVec2 filledCursorStart4 = ImVec2(filledCursorStart.x + 5.f, filledCursorStart.y + 20.f);
	//	ImVec2 filledCursorEnd4 = ImVec2(filledCursorStart.x + 35.f, filledCursorStart.y + 20.f);
	//	ImGui::GetWindowDrawList()->AddLine(filledCursorStart4, filledCursorEnd4, IM_COL32(200, 200, 200, 255));
	//}

	//void TextRenderer::DrawCenteringInspector(const ImColor& onColor, const ImColor& offColor)
	//{
	//	ImColor color;

	//	if (myTextCentering == 0)
	//		color = onColor;
	//	else
	//		color = offColor;

	//	ImVec2 upCursorStart = ImGui::GetCursorScreenPos();
	//	ImGui::PushStyleColor(ImGuiCol_Button, color.Value);
	//	if (ImGui::Button("##ButtonForUseWithCenterUp", ImVec2(40, 25)))
	//		myTextCentering = 0;
	//	ImGui::PopStyleColor();

	//	ImVec2 upCursorStart1 = ImVec2(upCursorStart.x + 5.f, upCursorStart.y + 4.f);
	//	ImVec2 upCursorEnd1 = ImVec2(upCursorStart.x + 35.f, upCursorStart.y + 4.f);
	//	ImGui::GetWindowDrawList()->AddLine(upCursorStart1, upCursorEnd1, IM_COL32(200, 200, 200, 255));

	//	ImVec2 upCursorStart2 = ImVec2(upCursorStart.x + 5.f, upCursorStart.y + 8.f);
	//	ImVec2 upCursorEnd2 = ImVec2(upCursorStart.x + 35.f, upCursorStart.y + 8.f);
	//	ImGui::GetWindowDrawList()->AddLine(upCursorStart2, upCursorEnd2, IM_COL32(200, 200, 200, 255));

	//	ImVec2 upCursorStart3 = ImVec2(upCursorStart.x + 5.f, upCursorStart.y + 12.f);
	//	ImVec2 upCursorEnd3 = ImVec2(upCursorStart.x + 35.f, upCursorStart.y + 12.f);
	//	ImGui::GetWindowDrawList()->AddLine(upCursorStart3, upCursorEnd3, IM_COL32(200, 200, 200, 255));

	//	ImGui::SameLine();

	//	if (myTextCentering == 1)
	//		color = onColor;
	//	else
	//		color = offColor;

	//	ImVec2 middleCursorStart = ImGui::GetCursorScreenPos();
	//	ImGui::PushStyleColor(ImGuiCol_Button, color.Value);
	//	if (ImGui::Button("##ButtonForUseWithCenterMiddle", ImVec2(40, 25)))
	//		myTextCentering = 1;
	//	ImGui::PopStyleColor();

	//	ImVec2 middleCursorStart1 = ImVec2(middleCursorStart.x + 5.f, middleCursorStart.y + 8.5);
	//	ImVec2 middleCursorEnd1 = ImVec2(middleCursorStart.x + 35.f, middleCursorStart.y + 8.5);
	//	ImGui::GetWindowDrawList()->AddLine(middleCursorStart1, middleCursorEnd1, IM_COL32(200, 200, 200, 255));

	//	ImVec2 middleCursorStart2 = ImVec2(middleCursorStart.x + 5.f, middleCursorStart.y + 12.5f);
	//	ImVec2 middleCursorEnd2 = ImVec2(middleCursorStart.x + 35.f, middleCursorStart.y + 12.5);
	//	ImGui::GetWindowDrawList()->AddLine(middleCursorStart2, middleCursorEnd2, IM_COL32(200, 200, 200, 255));

	//	ImVec2 middleCursorStart3 = ImVec2(middleCursorStart.x + 5.f, middleCursorStart.y + 16.5);
	//	ImVec2 middleCursorEnd3 = ImVec2(middleCursorStart.x + 35.f, middleCursorStart.y + 16.5);
	//	ImGui::GetWindowDrawList()->AddLine(middleCursorStart3, middleCursorEnd3, IM_COL32(200, 200, 200, 255));

	//	ImGui::SameLine();

	//	if (myTextCentering == 2)
	//		color = onColor;
	//	else
	//		color = offColor * 0.5f;

	//	ImVec2 downCursorStart = ImGui::GetCursorScreenPos();
	//	ImGui::PushStyleColor(ImGuiCol_Button, color.Value);
	//	if (ImGui::Button("##ButtonForUseWithCenterDown", ImVec2(40, 25)) && false)
	//		myTextCentering = 2;
	//	ImGui::PopStyleColor();

	//	ImVec2 downCursorStart1 = ImVec2(downCursorStart.x + 5.f, downCursorStart.y + 12.f);
	//	ImVec2 downCursorEnd1 = ImVec2(downCursorStart.x + 35.f, downCursorStart.y + 12.f);
	//	ImGui::GetWindowDrawList()->AddLine(downCursorStart1, downCursorEnd1, IM_COL32(200, 200, 200, 255));

	//	ImVec2 downCursorStart2 = ImVec2(downCursorStart.x + 5.f, downCursorStart.y + 16.f);
	//	ImVec2 downCursorEnd2 = ImVec2(downCursorStart.x + 35.f, downCursorStart.y + 16.f);
	//	ImGui::GetWindowDrawList()->AddLine(downCursorStart2, downCursorEnd2, IM_COL32(200, 200, 200, 255));

	//	ImVec2 downCursorStart3 = ImVec2(downCursorStart.x + 5.f, downCursorStart.y + 20.f);
	//	ImVec2 downCursorEnd3 = ImVec2(downCursorStart.x + 35.f, downCursorStart.y + 20.f);
	//	ImGui::GetWindowDrawList()->AddLine(downCursorStart3, downCursorEnd3, IM_COL32(200, 200, 200, 255));
	//}

	void TextRenderer::OnDrawGizmos()
	{
		// This is not right, does not work as intended
		
		if (drawRectGizmos)
		{
			auto tranform = gameObject->GetComponent<RectTransform>();
			if (!tranform)
				return;
			if (!tranform->myCanvas)
				return;
		
			Math::Vector2f resolution = tranform->myCanvas->ReferenceResolution;
			float aspectRatio = resolution.x / resolution.y;
			Math::Vector2f canvasScaleRelationOneDiv = { 1.f / resolution.x, 1.f / resolution.y };
		
			const Math::Vector2f& position = tranform->GetPosition() * canvasScaleRelationOneDiv * Math::Vector2f(aspectRatio, 1);
			//float rotation = gameObject->transform->GetRotation();
			Math::Vector2f scale = tranform->WidthHeightPX.Get() * myRect * canvasScaleRelationOneDiv;
			scale.x *= aspectRatio;

			//Math::Vector2f CanvasPosition = canvasScaleRelationOneDiv * tranform->myCanvas->canvasCameraTransform.PositionOffset * 0.5f;
		
			DebugDrawer::DrawSquare(position + Math::Vector2f(0.5f, 0.5f), 0.f, scale, Math::Color(0.7f, 0.7f, 0.7f, 1.f));
		}
	}

	void TextRenderer::Render()
	{
		CommandListManager::GetUICommandList().Enqueue([&]() {
				this->Draw();
			});
	}

	void TextRenderer::Draw()
	{
		if (!myMaterial)
			return;
		if (!font->data)
			return;

		const char* textInConstChar = myText->c_str();
		auto transform = gameObject->GetComponent<RectTransform>();

		if (!transform)
			return;
		if (!transform->myCanvas)
			return;

		// CameraBuffer* cameraBuffer = nullptr;
		// GraphicsEngine::Get<OpenGLGraphicsEngine>()->GetGraphicsBuffer()->GetBuffer<CameraBuffer>(cameraBuffer);
		
		transform->myCanvas->SetCanvasTransformProperties();
		
		unsigned shaderID = myMaterial->programID;
		glUseProgram(shaderID);
		
		Math::Vector2f resolution = transform->myCanvas->ReferenceResolution;

		auto CurrentFont = font->data->font;

		TransformUpdate();
		
		GraphicsEngine::Get<OpenGLGraphicsEngine>()->GetGraphicsBuffer()->SetOrCreateBuffer(1, myTransformBuffer);
		
		Math::Vector2f textOffset = { 0, 0 };
		Math::Vector2f scaleRect = myTransformBuffer.Scale * myRect * resolution;
		scaleRect.x *= 0.5f;
		
		lineOffsets.resize(1);
		lineOffsets.back() = 0;
		for (int i = 0; i < myText->size(); i++)
		{
			char character = textInConstChar[i];
			if (character == ' ')
			{
				float spaceOffset = myTransformBuffer.Scale.x * 50.f * 0.5f * myCharacterSpacing * myFontSize * mySpaceSpacing;
				lineOffsets.back() += spaceOffset;
				continue;
			}
			else if (character == '\n')
			{
				lineOffsets.emplace_back(0.f);
				continue;
			}
			Character& characterFace = CurrentFont.myCharTexture.at(character);
			float convertedAdvance = (float)(characterFace.advance >> 6) * 100.f * myTransformBuffer.Scale.x;
			lineOffsets.back() += convertedAdvance * 0.5f * myTextAlignment * myCharacterSpacing;
		}
		
		
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
		
		myTextMaterialBuffer.color = myTextColor;
		GraphicsEngine::Get<OpenGLGraphicsEngine>()->GetGraphicsBuffer()->SetOrCreateBuffer<TextMaterialBuffer>(5, myTextMaterialBuffer);

		
		int currentLineCount = 0;
		for (int i = 0; i < myText->size(); i++)
		{
			char character = textInConstChar[i];
		
			if (character == '\t')
			{
				textOffset.x += myTransformBuffer.Scale.x * 50.f * 4.f * myCharacterSpacing * myFontSize;
				continue;
			}
			else if (character == '\n')
			{
				textOffset.y -= myTransformBuffer.Scale.y * 50.f * myEnterSpacing * 2.f * myFontSize;
				textOffset.x = startOffset.x;
				currentLineCount++;
				continue;
			}
			else if (character == ' ')
			{
				textOffset.x += myTransformBuffer.Scale.x * 50.f * myCharacterSpacing * myFontSize * mySpaceSpacing;
				continue;
			}
		
		
			if (CurrentFont.myCharTexture.find(character) == CurrentFont.myCharTexture.end())
				character = '\n';
		
			Character& characterFace = CurrentFont.myCharTexture.at(character);
			float characterAdvance = (float)(characterFace.advance >> 6) * 100.f * myTransformBuffer.Scale.x;
		
			//Math::Vector2f scaleRect = myTransformBuffer.Scale * myRect * resolution;
			// float newXOffset = textOffset.x + characterAdvance;
			// if (newXOffset >= scaleRect.x)
			// {
			// 	return;
			// }
		
			myMaterial->Use(characterFace.textureID);
		
			myTextBuffer.size = { (float)characterFace.size.x, (float)characterFace.size.y};
		
			float lineOffset;
			if (myTextAlignment == 0)
				lineOffset = 0;
			else
				lineOffset = lineOffsets[currentLineCount] - myTransformBuffer.Scale.x * myCharacterSpacing;
		
		
			myTextBuffer.offset = textOffset;
			myTextBuffer.offset.x += (characterFace.bearing.x * 100.f * myTransformBuffer.Scale.x) - lineOffset;
			myTextBuffer.offset.y -= (characterFace.size.y - characterFace.bearing.y) * myTransformBuffer.Scale.y * 100.f;

#ifdef ECLIPSED_EDITOR
			EditorBuffer* editorBuffer;
			GraphicsEngine::Get<OpenGLGraphicsEngine>()->GetGraphicsBuffer()->GetBuffer<EditorBuffer>(editorBuffer);
			editorBuffer->PixelPickColor = gameObject->GetPixelPickingIDColor();
			GraphicsEngine::Get<OpenGLGraphicsEngine>()->GetGraphicsBuffer()->SetOrCreateBuffer<EditorBuffer>(35);
#endif
			
			GraphicsEngine::Get<OpenGLGraphicsEngine>()->GetGraphicsBuffer()->SetOrCreateBuffer(3, myTextBuffer);
			textOffset.x += characterAdvance * myCharacterSpacing;

			CanvasBuffer* canvasBuffer;
			GraphicsEngine::Get<OpenGLGraphicsEngine>()->GetGraphicsBuffer()->GetBuffer<CanvasBuffer>(canvasBuffer);
			
			if (!IsScene)
			{
				if (!transform->myCanvas->WorldSpace)
					canvasBuffer->canvasPositionOffset = transform->myCanvas->canvasCameraTransform.PositionOffset;
				else
					canvasBuffer->canvasPositionOffset = {0, 0};
            
			}
			
			GraphicsEngine::Get<OpenGLGraphicsEngine>()->GetGraphicsBuffer()->SetOrCreateBuffer(2, *canvasBuffer);
			
			TextSprite::Get().Render();
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
