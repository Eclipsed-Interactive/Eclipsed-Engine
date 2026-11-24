#include "Scene.h"

#include <AssetSystem/Assets/Data/SceneData.hpp>
#include <Renderer/RendererManager.h>

namespace Eclipse
{
	Scene::Scene(AssetSystem::AssetData* aData, GUID aGuid)
		: myGuid(aGuid), myData(reinterpret_cast<AssetSystem::SceneData*>(aData))
	{
	}

	std::string_view Scene::GetJson() const
	{
		return myData->json;
	}

	bool Scene::IsReady() const
	{
		return myData != nullptr;
	}
}
