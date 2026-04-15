#include "Camera.h"

#include "GraphicsEngine/OpenGL/OpenGLGraphicsAPI.h"

#include "EntityEngine/ComponentManager.h"

#include "EclipsedEngine/Components/Transform2D.h"

#include "GraphicsEngine/OpenGL/DebugDrawers/DebugDrawer.h"
#include "CoreEngine/MainSingleton.h"
#include "CoreEngine/Settings/EngineSettings.h"

namespace Eclipse
{
    void Camera::OnDestroy()
    {
        if (main == this)
            main = nullptr;
    }

    void Camera::OnComponentAdded()
    {
        OnSceneLoaded();
    }

    void Camera::UpdateCameraTransform()
    {
        if (Camera::main != this)
            return;

        myCameraBuffer.cameraPosition = gameObject->transform->GetPosition();
        myCameraBuffer.cameraRotation = gameObject->transform->GetRotation();
        myCameraBuffer.cameraScale = { CameraZoom, CameraZoom };

        GraphicsEngine::Get<OpenGLGraphicsEngine>()->GetGraphicsBuffer()->SetOrCreateBuffer(0, myCameraBuffer);

        // GraphicsEngine::Get<OpenGLGraphicsEngine>()->UpdateGlobalUniform(UniformType::Vector2f, "cameraPosition", &position);
        // GraphicsEngine::Get<OpenGLGraphicsEngine>()->UpdateGlobalUniform(UniformType::Float, "cameraRotation", &rotation);
        // GraphicsEngine::Get<OpenGLGraphicsEngine>()->UpdateGlobalUniform(UniformType::Vector2f, "cameraScale", &scale);
    }


    void Camera::EditorUpdate()
    {
        OnDrawGizmos();
        
        if (!created)
        {
            main = this;

            gameObject->transform->AddFunctionToRunOnDirtyUpdate(this, [&]() { UpdateCameraTransform(); });

            created = true;
        }
    }

    void Camera::OnDrawGizmos()
    {
        if (drawCameraGizmos)
        {
            Math::Vector2f sqrPosition = gameObject->transform->GetPosition() * 0.5f + Math::Vector2f(0.5f, 0.5f);
            float sqrRotation = gameObject->transform->GetRotation();
            Math::Vector2f sqrSize = Math::Vector2f(0.5f  * 1.7777777777f, 0.5f);

            DebugDrawer::DrawSquare(sqrPosition, sqrRotation, sqrSize, Math::Color(0.9f, 0.9f, 0.9f, 1.f));
        }
    }
}