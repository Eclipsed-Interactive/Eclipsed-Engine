#include "Texture.h"

namespace Eclipse
{
	// Temporary
	std::vector<Math::RectSizePos> Texture::GetRects() const
	{
		return data->spriteRects;
	}



	int Texture::GetWidth() const
	{
		return data->width;
	}

	int Texture::GetHeight() const
	{
		return data->height;
	}

	float Texture::GetAspectRatio() const
	{
		return (float)data->width / (float)data->height;
	}

	unsigned Texture::GetTextureID() const
	{
		return data->textureID;
	}

	const Math::Vector2f& Texture::GetTextureSizeNormilized() const
	{
		return data->sizeNormalized;
	}

	Math::Vector2f Texture::GetDimDivOne() const
	{
		return data->dimDivOne;
	}

	void Texture::Bind(int slot) const
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, data->textureID);
	}

	void Texture::Unbind(int slot) const
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}