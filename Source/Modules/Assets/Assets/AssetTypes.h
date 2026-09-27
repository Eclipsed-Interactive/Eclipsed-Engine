#pragma once

namespace Eclipse::Assets
{
	enum class DefaultAssetType
	{
		VERTEX_SHADER_UI,
		VERTEX_SHADER_SPRITE,

		TEXTURE_DEFAULT,
		TEXTURE_ERROR,

		MATERIAL_2D_SPRITE,
		MATERIAL_UI,
		MATERIAL_TEXT,
		PIXELPICK_MATERIAL,


		TEXT_FONT
	};

	enum class AssetType
	{
		Unknown,

		Texture,
		Material,
		VertexShader,
		PixelShader,
		AudioClip,
		Font,
		Prefab,
		Scene
	};
}