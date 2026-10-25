#pragma once

#include "Core/Standard/Serialization/SerializerRegistrator.hpp"
#include <charconv>

#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

rapidjson::Value BoolToJson(const bool& v, rapidjson::Document::AllocatorType& allocator)
{
	return rapidjson::Value(v);
}

void JsonToBool(void* v, rapidjson::Value& sv)
{
	bool* ptr = static_cast<bool*>(v);
	if (sv.IsBool()) *ptr = sv.GetBool();
}

REGISTER(bool, BoolToJson, JsonToBool);