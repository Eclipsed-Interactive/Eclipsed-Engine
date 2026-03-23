#include "Canvas.h"

#include "GraphicsEngine/OpenGL/DebugDrawers/DebugDrawer.h"
#include "GraphicsEngine/OpenGL/UniformVariableManager.h"

#include "GraphicsEngine/OpenGL/OpenGLGraphicsAPI.h"

namespace Eclipse
{
    void Canvas::SetCanvasTransformProperties()
    {
        int isScene;
        GraphicsEngine::Get<OpenGLGraphicsEngine>()->GetGlobalUniform(UniformType::Int, "IsSceneWindow", &isScene);

        CameraBuffer* cameraBuffer = nullptr;
        GraphicsEngine::Get<OpenGLGraphicsEngine>()->GetGraphicsBuffer()->GetBuffer<CameraBuffer>(cameraBuffer);
        
        //canvasCameraTransform.Rotation = gameObject->transform->GetRotation();

        canvasCameraTransform.ScaleMultiplier = Math::Vector2f(cameraBuffer->cameraScale.x * cameraBuffer->resolutionRatio * 1.7777777777f, cameraBuffer->cameraScale.y) * 2.f;

        canvasCameraTransform.PositionOffset = { 0.f, 0.f };
        if (isScene)
        {
            canvasCameraTransform.ScaleMultiplier *= gameObject->transform->GetScale();
            
            Math::Vector2f scenePosition = cameraBuffer->cameraPosition;
            canvasCameraTransform.PositionOffset = (scenePosition * -1.f) * ReferenceResolution;
        }

        if (isScene || WorldSpace)
        {
            canvasCameraTransform.PositionOffset += gameObject->transform->GetPosition() * ReferenceResolution;
            canvasCameraTransform.PositionOffset *= Math::Vector2f(cameraBuffer->resolutionRatio, 1.f) * cameraBuffer->cameraScale;
        }
        

        // Rotation of canvas fucks up many things
        //float sceneRotation;
        //GraphicsEngine::Get<OpenGLGraphicsEngine>()->GetGlobalUniform(UniformType::Float, "cameraRotation", &sceneRotation);
        //canvasCameraTransform.Rotation += sceneRotation;
    }

    void Canvas::EditorUpdate()
    {
        if (drawCanvasGizmos)
        {
            float sizeX = ReferenceResolution->x / 1080;
            float sizeY = ReferenceResolution->y / 1080;

            Math::Vector2f sqrPosition = gameObject->transform->GetPosition() * 0.5f + Math::Vector2f(0.5f, 0.5f);
            float sqrRotation = gameObject->transform->GetRotation();
            Math::Vector2f sqrSize = Math::Vector2f(0.5f * sizeX, 0.5f * sizeY);

            DebugDrawer::DrawSquare(sqrPosition, sqrRotation, sqrSize, Math::Color(0.9f, 0.9f, 0.9f, 1.f));
        }
    }

    void Canvas::OnComponentAdded()
    {
        gameObject->transform->AddFunctionToRunOnDirtyUpdate([&](){ TransformUpdate(); });
    }

    void Canvas::TransformUpdate()
    {
        Math::Vector2f resolution = ReferenceResolution;
        myCanvasBuffer.canvasScaleRelationOneDiv = { 1.f / resolution.x, 1.f / resolution.y };
        GraphicsEngine::Get<OpenGLGraphicsEngine>()->GetGraphicsBuffer()->SetOrCreateBuffer(2, myCanvasBuffer);
    }
}
