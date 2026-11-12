//#pragma once
//
//#include "Core/Math/Vector/Vector2.h"
//
//#include "Core/Standard/Serialization/SerializerRegistrator.hpp"
//#include <charconv>
//
//#include <rapidjson/stringbuffer.h>
//#include <rapidjson/writer.h>
//
//using namespace Eclipse::Math;
//
//using Vec2Bool = Vector2<bool>;
//
//rapidjson::Value Vector2fToJson(const Vector2f& v, rapidjson::Document::AllocatorType& allocator)
//{
//	rapidjson::Value obj(rapidjson::kObjectType);
//	obj.AddMember("x", v.x, allocator);
//	obj.AddMember("y", v.y, allocator);
//	return obj;
//}
//
//void JsonToVector2f(void* v, const rapidjson::Value& sv)
//{
//	if (!sv.IsObject()) return;
//
//	Vector2f* value = static_cast<Vector2f*>(v);
//
//	if (sv.HasMember("x") && sv["x"].IsNumber()) value->x = sv["x"].GetFloat();
//	if (sv.HasMember("y") && sv["y"].IsNumber()) value->y = sv["y"].GetFloat();
//}
//
//rapidjson::Value Vector2BoolToJson(const Vector2<bool>& v, rapidjson::Document::AllocatorType& allocator)
//{
//	rapidjson::Value obj(rapidjson::kObjectType);
//	obj.AddMember("x", v.x, allocator);
//	obj.AddMember("y", v.y, allocator);
//	return obj;
//}
//
//void JsonToVector2Bool(void* v, const rapidjson::Value& sv)
//{
//	if (!sv.IsObject()) return;
//
//	Vector2<bool>* value = static_cast<Vector2<bool>*>(v);
//
//	if (sv.HasMember("x") && sv["x"].IsBool()) value->x = sv["x"].GetBool();
//	if (sv.HasMember("y") && sv["y"].IsBool()) value->y = sv["y"].GetBool();
//}
//
//REGISTER(Vector2f, Vector2fToJson, JsonToVector2f);
//REGISTER(Vec2Bool, Vector2BoolToJson, JsonToVector2Bool);