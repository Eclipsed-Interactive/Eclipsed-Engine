#include "TextureData.h"

#include "OpenGL/GLFW/glfw3.h"

Eclipse::TextureData::~TextureData()
{
	if (textureID == 0) return;

	glDeleteTextures(1, &textureID);
}
