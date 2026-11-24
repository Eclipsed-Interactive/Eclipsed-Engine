#include "AssetSystem/MetaData/TextureMeta.h"

namespace Eclipse::AssetSystem
{
	void TextureMeta::SaveData(JsonWriter& writer) const
	{
		writer.Key("spriteRects");
		writer.StartArray();
		for (const Rect& r : spriteRects)
		{
			writer.StartObject();
			writer.Key("name");	writer.String(r.name.c_str());
			writer.Key("x");	writer.Double(r.rect.position.x);
			writer.Key("y");	writer.Double(r.rect.position.y);
			writer.Key("w");	writer.Double(r.rect.size.x);
			writer.Key("h");	writer.Double(r.rect.size.y);
			writer.EndObject();
		}
		writer.EndArray();
	}

	void TextureMeta::LoadData(const rapidjson::Value& data)
	{
		spriteRects.clear();
		if (!data.HasMember("spriteRects") || !data["spriteRects"].IsArray()) return;

		for (const auto& v : data["spriteRects"].GetArray())
		{
			if (!v.IsObject()) continue;

			Rect r;
			if (v.HasMember("name") && v["name"].IsString()) r.name = v["name"].GetString();
			if (v.HasMember("x") && v["x"].IsNumber()) r.rect.position.x = v["x"].GetFloat();
			if (v.HasMember("y") && v["y"].IsNumber()) r.rect.position.y = v["y"].GetFloat();
			if (v.HasMember("w") && v["w"].IsNumber()) r.rect.size.x = v["w"].GetFloat();
			if (v.HasMember("h") && v["h"].IsNumber()) r.rect.size.y = v["h"].GetFloat();
			spriteRects.push_back(r);
		}
	}
}
