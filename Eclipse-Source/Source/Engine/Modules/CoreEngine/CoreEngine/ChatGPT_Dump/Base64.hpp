#pragma once

#include <string>
#include <vector>

class Base64
{
public:
	static std::string Encode(const void* data, size_t len)
	{
		static const char* chars =
			"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
		const unsigned char* bytes = static_cast<const unsigned char*>(data);
		std::string result;
		result.reserve((len + 2) / 3 * 4);

		for (size_t i = 0; i < len; i += 3)
		{
			uint32_t triple = 0;
			size_t remain = len - i;

			triple |= bytes[i] << 16;
			if (remain > 1) triple |= bytes[i + 1] << 8;
			if (remain > 2) triple |= bytes[i + 2];

			result.push_back(chars[(triple >> 18) & 0x3F]);
			result.push_back(chars[(triple >> 12) & 0x3F]);
			result.push_back(remain > 1 ? chars[(triple >> 6) & 0x3F] : '=');
			result.push_back(remain > 2 ? chars[triple & 0x3F] : '=');
		}

		return result;
	}

	static std::vector<unsigned char> Decode(const std::string& str)
	{
		static const unsigned char table[256] = {
			64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,
			64,64,64,64,64,64,64,64,64,64,64,62,64,64,64,63,52,53,54,55,56,57,58,59,60,61,64,64,64,64,64,64,
			64, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,64,64,64,64,64,
			64,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,64,64,64,64,64,
			64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,
			64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,
			64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,
			64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64
		};

		std::vector<unsigned char> result;
		size_t len = str.size();
		size_t i = 0;

		while (i < len)
		{
			uint32_t triple = 0;
			int padding = 0;

			for (int j = 0; j < 4; ++j)
			{
				triple <<= 6;
				if (i < len)
				{
					unsigned char c = str[i++];
					if (c == '=') { padding++; }
					triple |= table[c];
				}
			}

			if (padding < 3) result.push_back((triple >> 16) & 0xFF);
			if (padding < 2) result.push_back((triple >> 8) & 0xFF);
			if (padding < 1) result.push_back(triple & 0xFF);
		}

		return result;
	}
};