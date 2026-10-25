#include "Serialization.hpp"

namespace Eclipse::Serialization
{
	inline const JsonSerializer& Serialization::GetJson(unsigned long long typeID)
	{
		if (!jsonRegistry.contains(typeID))
			throw std::runtime_error("Serializer not registered.");

		return jsonRegistry.at(typeID);
	}
}