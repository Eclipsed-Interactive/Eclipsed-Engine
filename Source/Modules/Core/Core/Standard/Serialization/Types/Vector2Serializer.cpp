#pragma once

#include "Core/Math/Vector/Vector2.h"

#include "Core/Standard/Serialization/SerializerRegistrator.hpp"
#include <charconv>

#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

using namespace Eclipse::Math;

rapidjson::Value Vector2fToJson(const Vector2f& v, rapidjson::Document::AllocatorType& allocator)
{
	rapidjson::Value obj(rapidjson::kObjectType);
	obj.AddMember("x", v.x, allocator);
	obj.AddMember("y", v.y, allocator);
	return obj;
}

void JsonToVector2f(void* v, rapidjson::Value& sv)
{
	if (!sv.IsObject()) return;

	Vector2f* value = static_cast<Vector2f*>(v);

	if (sv.HasMember("x") && sv["x"].IsNumber()) value->x = sv["x"].GetFloat();
	if (sv.HasMember("y") && sv["y"].IsNumber()) value->y = sv["y"].GetFloat();
}

REGISTER(Vector2f, Vector2fToJson, JsonToVector2f);