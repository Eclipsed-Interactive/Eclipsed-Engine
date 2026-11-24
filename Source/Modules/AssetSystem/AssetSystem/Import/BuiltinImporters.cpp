#include "BuiltinImporters.h"

#include "ImporterRegistry.h"
#include "Types/ShaderImporter.h"
#include "Types/TextureImporter.h"
#include "Types/MaterialImporter.h"
#include "Types/SceneImporter.h"

namespace Eclipse::AssetSystem
{
	ASSETSYSTEM_API void RegisterBuiltinImporters()
	{
		ImporterRegistry::Register<TextureImporter>(AssetType::Texture,
			{ ".png", ".jpg", ".jpeg", ".bmp", ".tga", ".psd", ".gif", ".hdr" }
		);

		ImporterRegistry::Register<ShaderImporter>(AssetType::Shader,
			{ ".glsl", ".pglsl", ".vglsl", ".cglsl", ".gglsl" }
		);

		ImporterRegistry::Register<MaterialImporter>(AssetType::Material,
			{ ".mat" }
		);

		ImporterRegistry::Register<SceneImporter>(AssetType::Scene,
			{ ".scene" }
		);
	}
}