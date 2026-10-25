#pragma once

#include "Core/Standard/Serialization/SerializerRegistrator.hpp"
#include <charconv>

#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

rapidjson::Value IntegerToJson(const int& v, rapidjson::Document::AllocatorType& allocator)
{
	return rapidjson::Value(v);
}

void JsonToInteger(void* v, rapidjson::Value& sv)
{
	int* ptr = static_cast<int*>(v);
	if (sv.IsInt()) *ptr = sv.GetInt();
}

REGISTER(int, IntegerToJson, JsonToInteger);