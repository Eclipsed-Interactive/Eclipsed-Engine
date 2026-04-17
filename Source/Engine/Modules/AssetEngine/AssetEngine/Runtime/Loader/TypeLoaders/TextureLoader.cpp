#include "TextureLoader.h"



namespace Eclipse
{
	TextureData* TextureLoader::Load(std::ifstream& in) const
	{
		TextureData* data = new TextureData;
		in.read(reinterpret_cast<char*>(&data->width), sizeof(int));
		in.read(reinterpret_cast<char*>(&data->height), sizeof(int));
		in.read(reinterpret_cast<char*>(&data->channels), sizeof(int));

		const int size = data->width * data->height * data->channels;
		unsigned char* pixelData = new unsigned char[size];
		in.read(reinterpret_cast<char*>(pixelData), size);

		int dataCount = 0;
		in.read(reinterpret_cast<char*>(&dataCount), sizeof(int));
		data->spriteRects.resize(dataCount);
		in.read(reinterpret_cast<char*>(data->spriteRects.data()), dataCount * sizeof(Math::RectSizePos));

		data->dimDivOne.X = 1.f / static_cast<float>(data->width);
		data->dimDivOne.Y = 1.f / static_cast<float>(data->height);
		data->sizeNormalized = Math::Vector2f{ 1.f, static_cast<float>(data->height) / data->width };

		glGenTextures(1, &data->textureID);
		glBindTexture(GL_TEXTURE_2D, data->textureID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, 0x2901);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, 0x2901);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, 0x2601);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, 0x2601);

		int rgbTypeOffset = 3 - data->channels;

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB - rgbTypeOffset, data->width, data->height, 0, GL_RGB - rgbTypeOffset, GL_UNSIGNED_BYTE, pixelData);
		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);

		delete[] pixelData;
		
		return data;
	}
}