#include "PointLight2D.h"

#include "EclipsedEngine/Components/Transform2D.h"
#include "EclipsedEngine/Components/Rendering/Camera.h"
#include "Core/GraphicsBuffers/EditorBuffer.h"

#include "EclipsedEngine/Components/Rendering/RenderCommands/Light2DCommand.h"
#include "Renderer/Sprite.h"
#include "Renderer/IRenderer.h"
#include "Renderer/IDrawer.h"
#include "Renderer/RendererManager.h"
#include "Renderer/RenderCommands/CommandList.h"

#include "Physics/PhysicsEngine.h"
#include <Core/GraphicsBuffers/LightBuffer.h>

namespace Eclipse
{
	void PointLight2D::OnComponentAdded()
	{
		myLightMaterial = Assets::AssetManager::LoadDefault<Assets::Material>(Assets::DefaultAssetType::MATERIAL_2D_LIGHT);
	}

	void PointLight2D::Render()
	{
		if (!gameObject->transform)
			return;

		Graphics::CommandListManager* commandListManager = MainSingleton::GetPointer<Graphics::CommandListManager>();
		commandListManager->GetLightCommandList().Enqueue<Light2DCommand>(this);
	}

	void PointLight2D::Draw(unsigned aProgramID)
	{
		Graphics::IRenderer& renderer = Graphics::RendererManager::GetRenderer();
		auto* buffer = renderer.GetGraphicsBuffer();

		TransformBuffer transform;
		transform.Position = gameObject->transform->GetPosition();
		transform.Scale = { myRadius * 100.f, myRadius * 100.f };
		transform.Rotation = 0.f;

		LightBuffer light;
		light.color = myColor;
		light.intensity = myIntensity;
		light.falloff = myFalloff;
		light.innerRadius = myInnerRadius;

		renderer.GetDevice()->BindMaterial(myLightMaterial);
		buffer->SetOrCreateBuffer(1, transform);
		buffer->SetOrCreateBuffer(6, light);

		renderer.GetDrawer()->DrawSprite();
	}
}
