#include "Scene.h"

#include <AssetSystem/Assets/Data/SceneData.hpp>
#include <Renderer/RendererManager.h>

namespace Eclipse
{
	Scene::Scene(AssetSystem::AssetData* aData)
		: myGuid(aData->guid), myData(reinterpret_cast<AssetSystem::SceneData*>(aData))
	{
	}

	std::string_view Scene::GetJson() const
	{
		return myData->json;
	}

	GUID Scene::GetGUID() const
	{
		return myData->guid;
	}

	bool Scene::IsReady() const
	{
		return myData != nullptr;
	}
}
