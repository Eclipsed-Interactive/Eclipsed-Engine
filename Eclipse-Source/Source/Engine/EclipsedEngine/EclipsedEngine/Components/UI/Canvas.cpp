#include "Canvas.h"

#include "GraphicsEngine/OpenGL/DebugDrawers/DebugDrawer.h"
#include "GraphicsEngine/OpenGL/UniformVariableManager.h"

#include "GraphicsEngine/OpenGL/OpenGLGraphicsAPI.h"

namespace Eclipse
{
    void Canvas::SetCanvasTransformProperties()
    {
        int isScene;
        GraphicsEngine::GetGlobalUniform(UniformType::Int, "IsSceneWindow", &isScene);
        float aspectRatio;
        GraphicsEngine::GetGlobalUniform(UniformType::Float, "resolutionRatio", &aspectRatio);

        Math::Vector2f sceneScale;
        GraphicsEngine::GetGlobalUniform(UniformType::Vector2f, "cameraScale", &sceneScale);

        //canvasCameraTransform.Rotation = gameObject->transform->GetRotation();

        canvasCameraTransform.ScaleMultiplier = Math::Vector2f(sceneScale.x * aspectRatio * 1.7777777777f, sceneScale.y) * 2.f;

        canvasCameraTransform.PositionOffset = { 0.f, 0.f };
        if (isScene)
        {
            canvasCameraTransform.ScaleMultiplier *= gameObject->transform->GetScale();

            Math::Vector2f scenePosition;
            GraphicsEngine::GetGlobalUniform(UniformType::Vector2f, "cameraPosition", &scenePosition);
            canvasCameraTransform.PositionOffset = (scenePosition * -1.f) * ReferenceResolution;
        }

        if (isScene || WorldSpace)
        {
            canvasCameraTransform.PositionOffset += gameObject->transform->GetPosition() * ReferenceResolution;
            canvasCameraTransform.PositionOffset *= Math::Vector2f(aspectRatio, 1.f) * sceneScale;
        }
        

        // Rotation of canvas fucks up many things
        //float sceneRotation;
        //GraphicsEngine::GetGlobalUniform(UniformType::Float, "cameraRotation", &sceneRotation);
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
}