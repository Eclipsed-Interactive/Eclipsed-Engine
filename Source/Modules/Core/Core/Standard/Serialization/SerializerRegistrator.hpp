#pragma once

#include "Serialization.hpp"

#include "rapidjson/document.h"

#define REGISTER(cls, toJson, fromJson) static ::Eclipse::Serialization::AutoSerializerRegistrator<cls> cls##_Registrator(&toJson, &fromJson);

namespace Eclipse::Serialization
{
	template<typename T>
	struct AutoSerializerRegistrator {
		explicit AutoSerializerRegistrator(rapidjson::Value(*toJson)(const T&, rapidjson::Document::AllocatorType& allocator), void(*fromJson)(void* v, rapidjson::Value& sv))
		{
			Serialization::RegisterJson<T>(toJson, fromJson);
		}
	};
}