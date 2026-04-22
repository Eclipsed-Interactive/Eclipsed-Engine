#include "GuidGenerator.h"

#include <random>

namespace Eclipse
{
	std::string GuidGenerator::Generate()
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<uint32_t> dis(0, 0xFFFFFFFF);

		std::stringstream ss;
		for (int i = 0; i < 4; ++i)
		{
			ss << std::hex << std::setw(8) << std::setfill('0') << dis(gen);
		}

		return ss.str();
	}
}