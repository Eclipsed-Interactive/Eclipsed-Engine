#include "BuiltinLoaders.h"

#include "AssetLoaderRegistry.h"
#include "Types/TextureLoader.h"
#include "Types/ShaderLoader.h"
#include "Types/MaterialLoader.h"

namespace Eclipse::AssetSystem
{
	ASSETSYSTEM_API void RegisterBuiltinLoaders()
	{
		AssetLoaderRegistry::Register<TextureLoader>();
		AssetLoaderRegistry::Register<ShaderLoader>();
		AssetLoaderRegistry::Register<MaterialLoader>();
	}
}