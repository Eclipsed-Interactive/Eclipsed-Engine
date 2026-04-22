#include "IDGenerator.h"

#include <unordered_map>
#include <random>

namespace Eclipse::Utilities
{
	unsigned IDGenerator::GetID()
	{
		static std::vector<unsigned> ids;

		std::random_device dev;
		std::mt19937 rng(dev());
		std::uniform_int_distribution<std::mt19937::result_type> dist6(0, UINT32_MAX);

		unsigned id = 0;
		do
		{
			id = dist6(rng);
		} while (std::find(ids.begin(), ids.end(), id) != ids.end());

		ids.push_back(id);

		return id;
	}
}