#include "TextureData.h"

#include "OpenGL/GLFW/glfw3.h"

namespace Eclipse::Assets
{
	TextureData::~TextureData()
	{
		if (textureID == 0) return;

		glDeleteTextures(1, &textureID);
	}
}