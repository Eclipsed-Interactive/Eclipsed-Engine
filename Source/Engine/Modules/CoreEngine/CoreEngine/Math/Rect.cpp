#include "pch.h"

#include "Rect.h"

namespace Eclipse::Math
{
	//rapidjson::Value Math::Rect::Save(rapidjson::Document::AllocatorType& allocator) const
	//{
	//	rapidjson::Value minRect(rapidjson::kObjectType);

	//	minRect.AddMember("x", min.x, allocator);
	//	minRect.AddMember("y", min.y, allocator);


	//	rapidjson::Value maxRect(rapidjson::kObjectType);

	//	maxRect.AddMember("width", max.x - min.x, allocator);
	//	maxRect.AddMember("height", max.y - min.y, allocator);


	//	rapidjson::Value val(rapidjson::kObjectType);

	//	val.AddMember("pos", minRect.Move(), allocator);
	//	val.AddMember("size", maxRect.Move(), allocator);

	//	return val;
	//}
	//void Rect::Load(const rapidjson::Value& aValue)
	//{
	//	const rapidjson::Value& position = aValue["pos"];
	//	const rapidjson::Value& size = aValue["size"];
	//	

	//	if (position.HasMember("x") && position["x"].IsNumber())
	//		min.x = static_cast<float>(position["x"].GetFloat());

	//	if (position.HasMember("y") && position["y"].IsNumber())
	//		min.y = static_cast<float>(position["y"].GetFloat());

	//	if (size.HasMember("width") && size["width"].IsNumber())
	//		max.x = min.x + static_cast<float>(size["width"].GetFloat());

	//	if (size.HasMember("height") && size["height"].IsNumber())
	//		max.y = min.y + static_cast<float>(size["height"].GetFloat());
	//}
}