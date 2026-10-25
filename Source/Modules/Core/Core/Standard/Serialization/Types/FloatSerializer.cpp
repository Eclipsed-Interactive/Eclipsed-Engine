#pragma once

#include "Core/Standard/Serialization/SerializerRegistrator.hpp"
#include <charconv>

#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

rapidjson::Value FloatToJson(const float& v, rapidjson::Document::AllocatorType& allocator)
{
	return rapidjson::Value(v);
}

void JsonToFloat(void* v, rapidjson::Value& sv)
{
	float* ptr = static_cast<float*>(v);
	if (sv.IsFloat()) *ptr = sv.GetFloat();
}

REGISTER(float, FloatToJson, JsonToFloat);