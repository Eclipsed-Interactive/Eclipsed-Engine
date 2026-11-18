#include "DirectionalLight2D.h"

#include "EclipsedEngine/Components/Transform2D.h"

#include <Core/MainSingleton.h>
#include <Core/GraphicsBuffers/LightBuffer.h>
#include "Renderer/RendererManager.h"
#include "Renderer/IRenderer.h"

namespace Eclipse
{
	void DirectionalLight2D::OnComponentAdded()
	{
		MainSingleton::RegisterInstance<DirectionalLight2D>(*this);
	}

	void DirectionalLight2D::Render()
	{
		Graphics::IRenderer& renderer = Graphics::RendererManager::GetRenderer();
		auto* buffer = renderer.GetGraphicsBuffer();

		DirectionalLightBuffer light;
		light.color = myColor;
		light.intensity = myIntensity;
		light.shadowLength = myShadowLength;
		light.shadowSteps = myShadowSteps;

		Transform2D* trans = gameObject->transform;
		const float rad = trans->GetRotation() ;
		light.rotation = Math::Vector2f(cosf(rad), sinf(rad));

		buffer->SetOrCreateBuffer(Graphics::GraphicsBufferEnum::g_DirectionalLightBuffer, 7, &light);
	}
}
