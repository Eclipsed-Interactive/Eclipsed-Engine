#include "MaterialImporter.h"

#include "AssetSystem/Import/ImportContext.h"
#include <Core/GUID.h>
#include <Core/Math/Color.h>

#include <rapidjson/document.h>
#include <Serialization/Serialization.hpp>

namespace Eclipse::AssetSystem
{
	namespace
	{
		GUID ReadGuid(const rapidjson::Value& obj, const char* key)
		{
			GUID guid;
			if (obj.HasMember(key) && obj[key].IsString() && obj[key].GetStringLength() > 0)
				guid.FromString(obj[key].GetString());

			return guid;
		}
	}

	bool MaterialImporter::Import(ImportContext& ctx) const
	{
		ArtifactWriter& writer = ctx.Out();

		std::ifstream in(ctx.GetPath(), std::ios::binary);
		if (!in) return false;

		std::stringstream ss;
		ss << in.rdbuf();
		const std::string json = ss.str();

		rapidjson::Document doc;
		doc.Parse(json.c_str());
		if (doc.HasParseError() || !doc.IsObject()) 
			return false;
		
		Math::Color color = { 1.f, 1.f, 1.f, 1.f };
		if (doc.HasMember("color") && doc["color"].IsObject())
			Serialization::Serialization::GetJson<Math::Color>().fromJson(&color, doc["color"]);

		GUID ps = ReadGuid(doc, "pixel_shader");
		GUID vs = ReadGuid(doc, "vertex_shader");
		GUID tex = ReadGuid(doc, "texture");

		writer.Write(&color, sizeof(Math::Color));
		writer.Write(&tex, sizeof(GUID));
		writer.Write(&ps, sizeof(GUID));
		writer.Write(&vs, sizeof(GUID));

		return true;
	}
}