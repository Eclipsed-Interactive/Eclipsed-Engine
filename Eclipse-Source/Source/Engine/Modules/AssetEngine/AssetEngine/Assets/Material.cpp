#include "Material.h"

namespace Eclipse
{
	Texture& Material::GetTexture()
	{
		return data->texture;
	}
	const Texture& Material::GetTexture() const
	{
		return data->texture;
	}

	VertexShader& Material::GetVertexShader()
	{
		return data->vertexShader;
	}
	const VertexShader& Material::GetVertexShader() const
	{
		return data->vertexShader;
	}

	PixelShader& Material::GetPixelShader()
	{
		return data->pixelShader;
	}

	const PixelShader& Material::GetPixelShader() const
	{
		return data->pixelShader;
	}

	unsigned Material::GetShaderProgramID() const
	{
		return data->programID;
	}

	void Material::BindTexture()
	{
		data->texture.Bind();
	}

	void Material::BindShader()
	{
		glUseProgram(data->programID);
	}

	void Material::BindColor()
	{
		myMaterialBuffer.color = data->color;
	}

	void Material::Use()
	{
		BindShader();
		BindTexture();
		BindColor();
	}

	void Material::Create()
	{

	}
}
