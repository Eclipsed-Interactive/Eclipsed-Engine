#pragma once

#include <vector>

namespace Eclipse
{
	class STB_Helper
	{
	public:
		static unsigned char* Load_Texture_STB(const char* aPath, int& width, int& height, int& channels, bool flipVertically = false);

		static unsigned char* Resize_STB(unsigned char* original, int originalWidth, int originalHeight, int newWidth, int newHeight, int channels);

		static void FreeData_STB(unsigned char* someData);
	};
}