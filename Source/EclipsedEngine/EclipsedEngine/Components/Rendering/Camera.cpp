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
        Camera* IntrenalCamera = MainSingleton::GetPointer<Camera>();

        if (IntrenalCamera == this)
            IntrenalCamera = nullptr;
    }

    void Camera::OnComponentAdded()
    {
        MainSingleton::AddInstance(this);

        OnSceneLoaded();
    }

    //void Camera::UpdateCameraTransform()
    //{
    //    if (MainSingleton::GetPointer<Camera>() != this)
    //        return;

    //    myCameraBuffer.cameraPosition = gameObject->transform->GetPosition();
    //    myCameraBuffer.cameraRotation = gameObject->transform->GetRotation();
    //    myCameraBuffer.cameraScale = { CameraZoom, CameraZoom };
    //}


    void Camera::EditorUpdate()
    {
        if (!created)
        {
            //gameObject->transform->AddFunctionToRunOnDirtyUpdate(this, [&]() { UpdateCameraTransform(); });

            created = true;
        }

        OnDrawGizmos();
        
        Math::Vector2f Position = gameObject->transform->GetPosition();
        Math::Vector2f Scale = { 1.7777f, 1.f };

        MinBoundsWorld = Position - Scale;
        MaxBoundsWorld = Position + Scale;
    }

    void Camera::OnDrawGizmos()
    {
        if (drawCameraGizmos)
        {
            DebugDrawer::DrawSquareMinMax(MinBoundsWorld, MaxBoundsWorld, Math::Color(0.9f, 0.9f, 0.9f, 1.f));
        }
    }
}