#include "MaterialLoader.h"

#include <AssetSystem/Assets/Data/MaterialData.hpp>
#include <Core/Standard/Core/Types.hpp>
#include "ArtifactHeader.h"

#include <Renderer/RendererManager.h>

#include "AssetSystem/Loading/AssetLoader.h"

namespace Eclipse::AssetSystem
{
	bool MaterialLoader::Load(ArtifactReader& aReader, AssetData* aData) const
	{
		MaterialData* tex = static_cast<MaterialData*>(aData);

		aReader.Read(&tex->color, sizeof(Math::Color));

		GUID texture;
		aReader.Read(&texture, sizeof(GUID));
		if(!texture.IsEmpty())
			tex->texture = AssetLoader::Load<Texture>(texture);


		GUID ps;
		aReader.Read(&ps, sizeof(GUID));
		if (!ps.IsEmpty())
			tex->ps = AssetLoader::Load<Shader>(ps);


		GUID vs;
		aReader.Read(&vs, sizeof(GUID));
		if (!vs.IsEmpty())
			tex->vs = AssetLoader::Load<Shader>(vs);

		return true;
	}
}