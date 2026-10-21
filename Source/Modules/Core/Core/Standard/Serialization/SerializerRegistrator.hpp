#pragma once

#include "Serialization.hpp"

#define REGISTER(cls, toJson, fromJson) static ::Eclipse::Serialization::AutoSerializerRegistrator<cls> cls##_Registrator(&toJson, &fromJson);

namespace Eclipse::Serialization
{
	template<typename T>
	struct AutoSerializerRegistrator {
		explicit AutoSerializerRegistrator(std::string(*toJson)(const T&), void(*fromJson)(void* v, std::string_view sv))
		{
			Serialization::RegisterJson<T>(toJson, fromJson);
		}
	};
}