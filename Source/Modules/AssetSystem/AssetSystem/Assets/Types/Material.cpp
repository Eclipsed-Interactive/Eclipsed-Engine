#include "Material.h"

#include <AssetSystem/Assets/Data/MaterialData.hpp>
#include <Renderer/RendererManager.h>

namespace Eclipse
{
	Material::Material(AssetSystem::AssetData* aData, GUID aGuid)
		: myGuid(aGuid), myData(reinterpret_cast<AssetSystem::MaterialData*>(aData))
	{
		unsigned int vsId = UINT_MAX;
		if (Shader vs = GetVertexShader(); vs.IsReady())
			vsId = vs.ShaderID();

		unsigned int psId = UINT_MAX;
		if (Shader ps = GetPixelShader(); ps.IsReady())
			psId = ps.ShaderID();

		myData->programID = Graphics::RendererManager::GetRenderer().GetDevice()->CreateShaderProgram(
			psId == UINT_MAX ? nullptr : &psId,
			vsId == UINT_MAX ? nullptr : &vsId
		);
	}

	const Math::Color& Material::Color() const
	{
		return myData->color;
	}

	Texture Material::GetTexture() const
	{
		return myData->texture;
	}

	Shader Material::GetPixelShader() const
	{
		return myData->ps;
	}

	Shader Material::GetVertexShader() const
	{
		return myData->vs;
	}

	bool Material::IsReady() const
	{
		return myData != nullptr;
	}

	void Material::Bind()
	{
		if (Texture tex = GetTexture(); tex.IsReady())
			tex.Bind();

		Graphics::RendererManager::GetRenderer().GetDevice()->BindShader(myData->programID);
	}
}
