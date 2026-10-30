#pragma once

#include <Assets/MaterialAsset.h>
#include "Assets/AssetManager.h"

#include "Core/Standard/Serialization/SerializerRegistrator.hpp"
#include <charconv>

#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

using namespace Eclipse::Assets;

rapidjson::Value MaterialToJson(const Material& v, rapidjson::Document::AllocatorType& allocator)
{
	rapidjson::Value obj(rapidjson::kObjectType);
	const std::string guid = v.IsValid() ? v.GetAssetID().ToString() : "";
	obj.AddMember("guid", rapidjson::Value(guid.c_str(), guid.size(), allocator), allocator);

	return obj;
}

void JsonToMaterial(void* v, const rapidjson::Value& sv)
{
    Material* clip = static_cast<Material*>(v);
    *clip = Material();

    if (!sv.IsObject() || !sv.HasMember("guid") || !sv["guid"].IsString())
        return;

    const std::string guidStr(sv["guid"].GetString(), sv["guid"].GetStringLength());
    if (guidStr.size() != 32) 
        return; 

    try
    {
        GUID guid;
        guid.FromString(guidStr);
        *clip = AssetManager::Load<Material>(guid);
    }
    catch (...)
    {
        // Asset was deleted or never imported: leave the field empty instead of failing the whole scene load.
    }
}

REGISTER(Material, MaterialToJson, JsonToMaterial);