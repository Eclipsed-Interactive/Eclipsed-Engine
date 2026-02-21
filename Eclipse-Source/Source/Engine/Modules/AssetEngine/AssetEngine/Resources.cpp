#include "Resources.h"

#include "CoreEngine/EventSystem/EventSystem.h"

namespace Eclipse
{
	VertexShaderManager Resources::vertexShaderManager;
	PixelShaderManager Resources::pixelShaderManager;
	MaterialManager Resources::materialManager;
	TextureManager Resources::textureManager;
	PrefabManager Resources::prefabManager;
	AudioClipManager Resources::audioClipManager;

	void Resources::Init()
	{
		EventSystem::Subscribe("UnloadAsset", Resources::TryCleanUp);
	}

	void Resources::TryCleanUp(const std::string& aGuid)
	{
		if(CleanUp<VertexShader>(aGuid)) return;
		else if(CleanUp<PixelShader>(aGuid)) return;
		else if(CleanUp<Material>(aGuid)) return;
		else if(CleanUp<Texture>(aGuid)) return;
		else if(CleanUp<Prefab>(aGuid)) return;
	}
}