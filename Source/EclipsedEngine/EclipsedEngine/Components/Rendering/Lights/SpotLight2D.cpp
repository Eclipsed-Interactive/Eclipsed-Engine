#include "SpotLight2D.h"

#include "EclipsedEngine/Components/Transform2D.h"
#include "EclipsedEngine/Components/Rendering/Camera.h"
#include "Core/GraphicsBuffers/EditorBuffer.h"

#include "EclipsedEngine/Components/Rendering/RenderCommands/SpotLight2DCommand.h"
#include "Renderer/Sprite.h"
#include "Renderer/IRenderer.h"
#include "Renderer/IDrawer.h"
#include "Renderer/RendererManager.h"
#include "Renderer/RenderCommands/CommandList.h"

#include "Physics/PhysicsEngine.h"
#include <Core/GraphicsBuffers/LightBuffer.h>

namespace Eclipse
{
	void SpotLight2D::OnComponentAdded()
	{
		myLightMaterial = Assets::AssetManager::LoadDefault<Assets::Material>(Assets::DefaultAssetType::MATERIAL_2D_SPOTLIGHT);
	}

	void SpotLight2D::Render()
	{
		if (!gameObject->transform)
			return;

		Graphics::CommandListManager* commandListManager = MainSingleton::GetPointer<Graphics::CommandListManager>();
		commandListManager->GetSpotLightCommandList().Enqueue<SpotLight2DCommand>(this);
	}

	void SpotLight2D::Draw(unsigned aProgramID)
	{
		Graphics::IRenderer& renderer = Graphics::RendererManager::GetRenderer();
		auto* buffer = renderer.GetGraphicsBuffer();

		TransformBuffer transform;
		transform.Position = gameObject->transform->GetPosition();
		transform.Scale = { myRadius * 100.f, myRadius * 100.f };
		transform.Rotation = gameObject->transform->GetRotation() + Math::pi;

		SpotLightBuffer light;
		light.color = myColor;
		light.intensity = myIntensity;
		light.falloff = myFalloff;
		light.innerRadius = myInnerRadius;
		light.cosInner = cosf(myInner * 0.5f * (Math::pi / 180.f));
		light.cosOuter = cosf(myOuter * 0.5f * (Math::pi / 180.f));

		renderer.GetDevice()->BindMaterial(myLightMaterial);
		buffer->SetOrCreateBuffer(1, transform);
		buffer->SetOrCreateBuffer(6, light);

		renderer.GetDrawer()->DrawSprite();
	}
}
