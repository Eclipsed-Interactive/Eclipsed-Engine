#include "Camera.h"

#include "ECS/ComponentManager.h"

#include "EclipsedEngine/Components/Transform2D.h"

//#include "Renderer/OpenGL/DebugDrawers/DebugDrawer.h"
#include "Core/MainSingleton.h"
#include "Core/Settings/EngineSettings.h"

#include "Renderer/IRenderer.h"
#include "Renderer/RendererManager.h"

namespace Eclipse
{
    void Camera::OnDestroy()
    {
        if (main == this)
            main = nullptr;
    }

    void Camera::OnComponentAdded()
    {
        main = this;

        OnSceneLoaded();
    }

    void Camera::UpdateCameraTransform()
    {
        if (Camera::main != this)
            return;

        Graphics::IGraphicsBuffer* buffer = Graphics::RendererManager::GetRenderer().GetGraphicsBuffer();
        myCameraBuffer.cameraPosition = gameObject->transform->GetPosition();
        myCameraBuffer.cameraRotation = gameObject->transform->GetRotation();
        myCameraBuffer.cameraScale = { CameraZoom, CameraZoom };

        buffer->SetOrCreateBuffer(0, myCameraBuffer);
    }


    void Camera::EditorUpdate()
    {
        OnDrawGizmos();
        
        if (!created)
        {
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

            //DebugDrawer::DrawSquare(sqrPosition, sqrRotation, sqrSize, Math::Color(0.9f, 0.9f, 0.9f, 1.f));
        }
    }
}