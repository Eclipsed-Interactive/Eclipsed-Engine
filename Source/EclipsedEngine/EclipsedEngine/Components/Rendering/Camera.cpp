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
		if (MainSingleton::Exists<Camera>() && MainSingleton::GetPointer<Camera>() == this)
			MainSingleton::RemoveInstance<Camera>();
	}

	void Camera::OnComponentAdded()
	{
		//MainSingleton::AddInstance(this);
		MainSingleton::AddInstance<Camera>(this);

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

		Math::Vector2f Position = gameObject->transform->GetPosition();
		Math::Vector2f Scale = { 1.7777f, 1.f };

		minBoundsWorld = Position - Scale;
		maxBoundsWorld = Position + Scale;

		OnDrawGizmos();
	}

	void Camera::OnDrawGizmos()
	{
		if (DrawCameraGizmos)
		{
			DebugDrawer::DrawSquareMinMax(minBoundsWorld, maxBoundsWorld, Math::Color(0.9f, 0.9f, 0.9f, 1.f));
		}
	}
}