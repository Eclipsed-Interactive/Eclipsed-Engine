#pragma once

#include "Core/Standard/Serialization/SerializerRegistrator.hpp"
#include <charconv>

std::string FloatToJson(const float& v)
{
	return std::to_string(v);
}

void JsonToFloat(void* v, std::string_view sv)
{
	float* val = reinterpret_cast<float*>(v);
	/*std::from_chars_result result =*/ std::from_chars(sv.data(), sv.data() + sv.size(), *val);
	// Handle errors.
}

REGISTER(float, FloatToJson, JsonToFloat);