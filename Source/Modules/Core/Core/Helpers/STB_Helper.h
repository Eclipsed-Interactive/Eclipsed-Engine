#pragma once

#include <vector>

#include "Core.Core.hpp"

namespace Eclipse
{
	class CORE_API STB_Helper
	{
	public:
		static unsigned char* Load_Texture_STB(const char* aPath, int& width, int& height, int& channels, int reqChannels = 4, bool flipVertically = true);

		static unsigned char* Resize_STB(unsigned char* original, int originalWidth, int originalHeight, int newWidth, int newHeight, int channels);

		static void FreeData_STB(unsigned char* someData);

		static void SetFlipVerticalOnLoad_STB(bool flip);
	};
}