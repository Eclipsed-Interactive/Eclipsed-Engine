#include "AmbientLight2D.h"

#include <Core/MainSingleton.h>
#include <Core/GraphicsBuffers/LightBuffer.h>
#include "Renderer/RendererManager.h"
#include "Renderer/IRenderer.h"

namespace Eclipse
{
	void AmbientLight2D::OnComponentAdded()
	{
		MainSingleton::RegisterInstance<AmbientLight2D>(*this);
	}

	void AmbientLight2D::Render()
	{
		Graphics::IRenderer& renderer = Graphics::RendererManager::GetRenderer();
		auto* buffer = renderer.GetGraphicsBuffer();

		AmbientLightBuffer light;
		light.color = myColor;
		light.intensity = myIntensity;

		buffer->SetOrCreateBuffer(8, light);
	}
}
