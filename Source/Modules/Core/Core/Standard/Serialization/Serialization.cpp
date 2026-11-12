#include "Serialization.hpp"

namespace Eclipse::Serialization
{
	void Serialization::RegisterJson(unsigned long long typeId, JsonSerializer serializer)
	{
		jsonRegistry.insert_or_assign(typeId, std::move(serializer));
	}

	inline const JsonSerializer& Serialization::GetJson(unsigned long long typeID)
	{
		if (!jsonRegistry.contains(typeID))
			throw std::runtime_error("Serializer not registered.");

		return jsonRegistry.at(typeID);
	}
}