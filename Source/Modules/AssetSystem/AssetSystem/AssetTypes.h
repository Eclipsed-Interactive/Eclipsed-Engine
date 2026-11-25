#pragma once

namespace Eclipse::AssetSystem
{
	enum class DefaultAssetType
	{
		VERTEX_SHADER_UI,
		VERTEX_SHADER_SPRITE,

		TEXTURE_DEFAULT,
		TEXTURE_ERROR,

		MATERIAL_2D_LIGHTS_COMPOSITE,
		MATERIAL_2D_POINTLIGHT,
		MATERIAL_2D_SPOTLIGHT,

		MATERIAL_2D_SPRITE,
		MATERIAL_UI,
		MATERIAL_TEXT,
		PIXELPICK_MATERIAL,
		MATERIAL_2D_INSTANCE_SPRITE,

		TEXT_FONT
	};

	enum class AssetType
	{
		Unknown,

		Texture,
		Material,
		Shader,
		Audio,
		Font,
		Prefab,
		Scene
	};
}