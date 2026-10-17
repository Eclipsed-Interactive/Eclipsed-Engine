#include "Canvas.h"

#include "Renderer/UniformVariableManager.h"

#include "Core/GraphicsBuffers/CameraBuffer.h"
#include "Core/GraphicsBuffers/EditorBuffer.h"

#include "EclipsedEngine/Components/Transform2D.h"
#include "EclipsedEngine/Components/UI/RectTransform.h"

#include "Renderer/IRenderer.h"
#include "Renderer/RendererManager.h"

namespace Eclipse
{
	void Canvas::SetCanvasTransformProperties()
	{
		Graphics::IGraphicsBuffer* buffer = Graphics::RendererManager::GetRenderer().GetGraphicsBuffer();

		CameraBuffer* cameraBuffer = nullptr;
		buffer->GetBuffer(cameraBuffer);


#ifdef ECL_EDITOR
		EditorBuffer* editorBuffer = nullptr;
		buffer->GetBuffer(editorBuffer);

		if (editorBuffer->IsSceneView)
			canvasCameraTransform.ScaleMultiplier = Math::Vector2f(cameraBuffer->cameraScale.x * cameraBuffer->resolutionRatio * 1.7777777777f, cameraBuffer->cameraScale.y) * 2.f;
		else
#endif
			canvasCameraTransform.ScaleMultiplier = Math::Vector2f(cameraBuffer->resolutionRatio * 1.7777777777f, 1) * 2.f;



		canvasCameraTransform.ScaleMultiplier *= gameObject->transform->GetScale();
		Math::Vector2f scenePosition = cameraBuffer->cameraPosition;
		canvasCameraTransform.PositionOffset = (scenePosition * -1.f) * ReferenceResolution;


		canvasCameraTransform.PositionOffset += gameObject->transform->GetPosition() * ReferenceResolution;
		canvasCameraTransform.PositionOffset *= Math::Vector2f(cameraBuffer->resolutionRatio, 1.f) * cameraBuffer->cameraScale;


		// Rotation of canvas fucks up many things
		//float sceneRotation;
		//GraphicsEngine::Get()->GetGlobalUniform(UniformType::Float, "cameraRotation", &sceneRotation);
		//canvasCameraTransform.Rotation += sceneRotation;
	}

	void Canvas::EditorUpdate()
	{
#ifdef ECL_EDITOR
		if (drawCanvasGizmos)
		{
			Math::Vector2f Position = gameObject->transform->GetPosition();
			Math::Vector2f Scale(1.7777f, 1.f);

			DebugDrawer::DrawSquareMinMax(Position - Scale, Position + Scale, Math::Color(0.9f, 0.9f, 0.9f, 1.f));
		}
#endif
	}

	void Canvas::OnComponentAdded()
	{
		gameObject->transform->AddFunctionToRunOnDirtyUpdate(this, [&]() { TransformUpdate(); });

		SetCanvasForChildren(gameObject->GetChildren());
	}

	void Canvas::TransformUpdate()
	{
		Math::Vector2f resolution = ReferenceResolution;
		myCanvasBuffer.canvasScaleRelationOneDiv = { 1.f / resolution.x, 1.f / resolution.y };

		Graphics::IGraphicsBuffer* buffer = Graphics::RendererManager::GetRenderer().GetGraphicsBuffer();
		buffer->SetOrCreateBuffer(2, myCanvasBuffer);
	}

	void Canvas::SetCanvasForChildren(std::vector<GameObject*>& Children)
	{
		// To set the transformCanvas
		for (GameObject* child : Children)
		{
			RectTransform* recttransform = child->GetComponent<RectTransform>();
			if (!recttransform)
				return;

			recttransform->myCanvas = this;

			recttransform->myCanvas->canvasCameraTransform.PositionOffset = { 0.f, 0.f };
			recttransform->myCanvas->canvasCameraTransform.Rotation = 0.f;
			recttransform->myCanvas->canvasCameraTransform.ScaleMultiplier = { 1.f, 1.f };

			if (child->HasChildren())
				SetCanvasForChildren(child->GetChildren());
		}
	}
}
