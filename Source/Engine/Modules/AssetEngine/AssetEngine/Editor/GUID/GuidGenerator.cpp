#include "GuidGenerator.h"

#include <random>

namespace Eclipse
{
	size_t GuidGenerator::Generate(const std::string& someData)
	{
		size_t hash = 0xcbf29ce484222325;
		for (auto& byte : someData)
		{
			hash ^= byte;
			hash *= 0x100000001b3;
		}

		return hash;

		//std::random_device rd;
		//std::mt19937 gen(rd());
		//std::uniform_int_distribution<uint32_t> dis(0, 0xFFFFFFFF);

		//std::stringstream ss;
		//for (int i = 0; i < 4; ++i)
		//{
		//	ss << std::hex << std::setw(8) << std::setfill('0') << dis(gen);
		//}

		//return ss.str();
	}
}