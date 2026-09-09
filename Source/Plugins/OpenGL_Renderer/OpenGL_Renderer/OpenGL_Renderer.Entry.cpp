#include "OpenGL_Renderer.Entry.h"

#include "OpenGL_Renderer.h"

#include "Assets/AssetTypeRegistry.h"

#include "Texture/OpenGL_TextureAssetType.h"
#include "Material/OpenGL_MaterialAssetType.h"
#include "VertexShader/OpenGL_VertexShaderAssetType.h"
#include "PixelShader/OpenGL_PixelShaderAssetType.h"

using namespace Eclipse::Graphics;
using namespace Eclipse::Assets;
using namespace Eclipse::Graphics::OpenGL;

OPENGL_RENDERER_API IRenderer* CreateRenderer()
{
	AssetTypeRegistry::RegisterType(Eclipse::Assets::AssetType::Texture, new OpenGL_TextureAssetType);
	AssetTypeRegistry::RegisterType(Eclipse::Assets::AssetType::VertexShader, new OpenGL_VertexShaderAssetType);
	AssetTypeRegistry::RegisterType(Eclipse::Assets::AssetType::PixelShader, new OpenGL_PixelShaderAssetType);
	AssetTypeRegistry::RegisterType(Eclipse::Assets::AssetType::Material, new OpenGL_MaterialAssetType);

	return new OpenGL_Renderer;
}

OPENGL_RENDERER_API void DestroyRenderer(IRenderer* renderer)
{
	// Needs to be implemented.
}
