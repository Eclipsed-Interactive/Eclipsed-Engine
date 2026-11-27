#include "SceneLoader.h"

#include <AssetSystem/Assets/Data/SceneData.hpp>
#include <Core/Standard/Core/Types.hpp>
#include "ArtifactHeader.h"

#include <Renderer/RendererManager.h>

namespace Eclipse::AssetSystem
{
	bool SceneLoader::Load(ArtifactReader& aReader, AssetData* aData) const
	{
		SceneData* scene = static_cast<SceneData*>(aData);

		Size size = 0;
		if (!aReader.Read(&size, sizeof(Size))) return false;

		scene->json.resize(size);
		if (!aReader.Read(scene->json.data(), size)) return false;

		return true;
	}
	bool SceneLoader::Unload(AssetData* aData) const
	{
		SceneData* scene = static_cast<SceneData*>(aData);
		scene->json.clear();
		scene->json.shrink_to_fit();

		return true;
	}
}