#pragma once

#include "Core/Math/Color.h"

#include "Core/Standard/Serialization/SerializerRegistrator.hpp"
#include <charconv>

#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

using namespace Eclipse::Math;

rapidjson::Value ColorToJson(const Color& v, rapidjson::Document::AllocatorType& allocator)
{
	rapidjson::Value obj(rapidjson::kObjectType);
	obj.AddMember("r", v.r, allocator);
	obj.AddMember("g", v.g, allocator);
	obj.AddMember("b", v.b, allocator);
	obj.AddMember("a", v.a, allocator);
	return obj;
}

void JsonToColor(void* v, const rapidjson::Value& sv)
{
	if (!sv.IsObject()) return;

	Color* value = static_cast<Color*>(v);

	if (sv.HasMember("r") && sv["r"].IsNumber()) value->r = sv["r"].GetFloat();
	if (sv.HasMember("g") && sv["g"].IsNumber()) value->g = sv["g"].GetFloat();
	if (sv.HasMember("b") && sv["b"].IsNumber()) value->b = sv["b"].GetFloat();
	if (sv.HasMember("a") && sv["a"].IsNumber()) value->a = sv["a"].GetFloat();
}

REGISTER(Color, ColorToJson, JsonToColor);