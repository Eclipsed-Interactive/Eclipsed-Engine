#pragma once

#include <Assets/Types/Shader.h>
#include "Assets.h"

#include "Serialization/SerializerRegistrator.hpp"
#include <charconv>

#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

using namespace Eclipse;

rapidjson::Value ShaderToJson(const Shader& v, rapidjson::Document::AllocatorType& allocator)
{
	rapidjson::Value obj(rapidjson::kObjectType);
	const std::string guid = v.IsReady() ? v.GetGUID().ToString() : "";
	obj.AddMember("guid", rapidjson::Value(guid.c_str(), guid.size(), allocator), allocator);

	return obj;
}

void JsonToShader(void* v, const rapidjson::Value& sv)
{
    Shader* clip = static_cast<Shader*>(v);
    *clip = Shader();

    if (!sv.IsObject() || !sv.HasMember("guid") || !sv["guid"].IsString())
        return;

    const std::string guidStr(sv["guid"].GetString(), sv["guid"].GetStringLength());
    if (guidStr.size() != 32) 
        return; 

    try
    {
        GUID guid;
        guid.FromString(guidStr);
        *clip = Assets::Load<Shader>(guid);
    }
    catch (...)
    {
        // Asset was deleted or never imported: leave the field empty instead of failing the whole scene load.
    }
}

REGISTER(Shader, ShaderToJson, JsonToShader);