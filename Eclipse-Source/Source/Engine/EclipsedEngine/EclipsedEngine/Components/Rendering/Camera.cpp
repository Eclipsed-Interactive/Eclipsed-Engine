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
        myCameraBuffer.cameraScale = gameObject->transform->GetScale();

        GraphicsEngine::Get<OpenGLGraphicsEngine>()->GetGraphicsBuffer()->SetOrCreateBuffer(0, myCameraBuffer);

        // GraphicsEngine::Get<OpenGLGraphicsEngine>()->UpdateGlobalUniform(UniformType::Vector2f, "cameraPosition", &position);
        // GraphicsEngine::Get<OpenGLGraphicsEngine>()->UpdateGlobalUniform(UniformType::Float, "cameraRotation", &rotation);
        // GraphicsEngine::Get<OpenGLGraphicsEngine>()->UpdateGlobalUniform(UniformType::Vector2f, "cameraScale", &scale);
    }


    void Camera::OnSceneLoaded()
    {
        if (!created)
        {
            main = this;

            gameObject->transform->AddFunctionToRunOnDirtyUpdate([&]() { UpdateCameraTransform(); });

            created = true;
        }
    }

    void Camera::Render()
    {
        if (drawCameraGizmos)
        {
            //MainSingleton::GetInstance<EngineSettings>();

            float size = 1;

            Math::Vector2f sqrPosition = gameObject->transform->GetPosition() * 0.5f + Math::Vector2f(0.5f, 0.5f);
            float sqrRotation = gameObject->transform->GetRotation();
            Math::Vector2f sqrSize = Math::Vector2f(0.5f * size + 0.001f, 0.5f + 0.001f);

            DebugDrawer::DrawSquare(sqrPosition, sqrRotation, sqrSize, Math::Color(0.9f, 0.9f, 0.9f, 1.f));
        }
    }
}