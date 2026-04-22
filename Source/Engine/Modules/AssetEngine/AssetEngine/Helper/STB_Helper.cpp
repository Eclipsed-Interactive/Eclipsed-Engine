#include "STB_Helper.h"

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"
#endif // !STB_IMAGE_IMPLEMENTATION

#ifndef STB_IMAGE_RESIZE_IMPLEMENTATION
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image/stb_image_resize.h"
#endif // !STB_IMAGE_RESIZE_IMPLEMENTATION

namespace Eclipse
{
	unsigned char* STB_Helper::Load_Texture_STB(const char* aPath, int& width, int& height, int& channels, bool flipVertically)
	{
		stbi_set_flip_vertically_on_load(flipVertically);
		return stbi_load(aPath, &width, &height, &channels, 0);
	}

	unsigned char* STB_Helper::Resize_STB(unsigned char* original, int originalWidth, int originalHeight, int newWidth, int newHeight, int channels)
	{
		unsigned char* resized = new unsigned char[newWidth * newHeight * channels];
		int ok = stbir_resize_uint8(original, originalWidth, originalHeight, 0,
			resized, newWidth, newHeight, 0,
			channels);

		stbi_image_free(original);

		return resized;
	}

	void STB_Helper::FreeData_STB(unsigned char* someData)
	{
		if (!someData) return;

		stbi_image_free(someData);
	}
}