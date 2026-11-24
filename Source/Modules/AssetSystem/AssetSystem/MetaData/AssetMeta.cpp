#include "AssetMeta.h"

#include <fstream>
#include <filesystem>

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/istreamwrapper.h"
#include "SupportedAssets.h"

#include "AssetSystem/SourceMount.hpp"
#include "AssetSystem/MetaData/TextureMeta.h"
#include <AssetSystem/SourceMount.hpp>

namespace Eclipse::AssetSystem
{
	void AssetMeta::SetArtifactPath(const std::filesystem::path& anArtifactPath)
	{
		artifactPath = anArtifactPath;
	}

	AssetMeta* AssetMeta::CreateEmpty(const std::filesystem::path& aSource)
	{
		switch (GetAssetTypeFromExtension(aSource.extension().string()))
		{
		case AssetType::Texture: return new TextureMeta;
		default: break;
		}

		return new AssetMeta;
	}

	std::filesystem::path AssetMeta::GetMetaPath(const std::filesystem::path& aPath)
	{
		return aPath.generic_string() + ".meta";
	}

	void AssetMeta::Save()
	{
		using namespace rapidjson;

		StringBuffer buffer;
		JsonWriter writer(buffer);

		writer.StartObject();

		writer.Key("guid");		writer.String(guid.ToString().c_str());
		writer.Key("filename");	writer.String(filename.c_str());
		writer.Key("path");		writer.String(virtualPath.Get().c_str());
		writer.Key("type");		writer.Int((int)type);

		writer.Key("data");
		writer.StartObject();
		SaveData(writer);
		writer.EndObject();
		writer.EndObject();

		std::ofstream out(GetMetaPath(sourcePath), std::ios::binary);
		out.write(reinterpret_cast<const char*>(buffer.GetString()), buffer.GetSize());
	}

	bool AssetMeta::Load(const SourceMount& aMount, const std::filesystem::path& aSource)
	{
		using namespace rapidjson;

		sourcePath = aSource;

		auto t = GetMetaPath(sourcePath);
		std::ifstream in(GetMetaPath(sourcePath), std::ios::binary);
		if (!in)
		{
			virtualPath = AssetPath(aMount.scheme.c_str(), std::filesystem::relative(sourcePath, aMount.sourceLocation));
			guid.Generate();

			const std::string guidStr = guid.ToString();
			artifactPath = aMount.artifactLocation / guidStr.substr(0, 2) / guidStr;

			Save();

			return false;
		}

		IStreamWrapper isw(in);
		Document doc;
		doc.ParseStream(isw);
		if (doc.HasParseError() || !doc.IsObject()) return false;

		if (doc.HasMember("guid") && doc["guid"].IsString()) guid.FromString(doc["guid"].GetString());
		const std::string guidStr = guid.ToString();
		artifactPath = aMount.artifactLocation / guidStr.substr(0, 2) / guidStr;


		if (doc.HasMember("filename") && doc["filename"].IsString()) filename = doc["filename"].GetString();
		if (doc.HasMember("path") && doc["path"].IsString()) virtualPath = AssetPath(doc["path"].GetString());
		if (doc.HasMember("type") && doc["type"].IsInt()) type = (AssetType)doc["type"].GetInt();

		if (doc.HasMember("data") && doc["data"].IsObject())
			LoadData(doc["data"]);

		return true;
	}
}